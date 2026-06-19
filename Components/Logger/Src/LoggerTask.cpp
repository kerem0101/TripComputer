#include "LoggerTask.hpp"
#include <stdio.h>
#include <string.h>

LoggerTask::LoggerTask() : Thread("LoggerTask", 448, 1) { // Normal priority, 448 words stack (FatFs needs ~400B internally)
    _logQueue = NULL;
}

void LoggerTask::init() {
    // Create a queue that can hold up to 3 LogData items (RAM is very limited on STM32F0)
    _logQueue = osMessageQueueNew(3, sizeof(LogData), NULL);
}

bool LoggerTask::pushData(const LogData& data) {
    if (_logQueue == NULL) return false;
    // Push with 0 timeout (non-blocking for NavigationTask)
    return (osMessageQueuePut(_logQueue, &data, 0, 0) == osOK);
}

// Global variable for debugging SD card mount status via ST-Link
volatile FRESULT sd_mount_res = FR_NOT_READY;

// Flag to indicate SD card is busy (drawing power), to prevent LCD updates during voltage sag
volatile bool isSdCardBusy = false;

volatile char sd_test_read_buffer[256] = {0};

extern "C" {
    extern volatile uint8_t sd_debug_cmd0;
    extern volatile uint8_t sd_debug_cmd8;
    extern volatile uint8_t sd_debug_acmd41;
    extern volatile uint8_t sd_debug_timeout;
    extern volatile uint8_t sd_debug_cardtype;
}

void LoggerTask::run() {
    FRESULT fr;
    
    // Wait for SD card to fully power up before first access
    // (CMD0 was returning 0x3F garbage without this delay)
    vTaskDelay(pdMS_TO_TICKS(1000));
    
    // 1. Mount the SD card File System
    sd_mount_res = f_mount(&_fs, "", 1); // 1 = mount immediately
    bool isMounted = (sd_mount_res == FR_OK);
    
    if (isMounted) {
        fr = f_open(&_file, "trip_log.csv", FA_READ);
        if (fr == FR_OK) {
            UINT br;
            f_read(&_file, (void*)sd_test_read_buffer, sizeof(sd_test_read_buffer) - 1, &br);
            sd_test_read_buffer[br] = '\0';
            f_close(&_file);
        } else {
            snprintf((char*)sd_test_read_buffer, sizeof(sd_test_read_buffer), "f_open failed: %d", fr);
        }
    } else {
        snprintf((char*)sd_test_read_buffer, sizeof(sd_test_read_buffer), 
                 "fail:%d (c0:%d c8:%d a41:%d to:%d ty:%d)", 
                 sd_mount_res, sd_debug_cmd0, sd_debug_cmd8, sd_debug_acmd41, sd_debug_timeout, sd_debug_cardtype);
    }

    bool headerWritten = false;
    if (isMounted) {
        isSdCardBusy = true;
        fr = f_open(&_file, "trip_log.csv", FA_OPEN_ALWAYS | FA_WRITE);
        if (fr == FR_OK) {
            f_lseek(&_file, f_size(&_file));
            if (f_size(&_file) == 0) {
                const char* header = "Lat,Lon,Speed(km/h),Alt(m),Satellites,AccelX(g),AccelY(g),AccelZ(g),GyroX(dps),GyroY(dps),GyroZ(dps)\n";
                UINT bw;
                f_write(&_file, header, strlen(header), &bw);
            }
            f_close(&_file);
            headerWritten = true;
        }
        isSdCardBusy = false;
    }

    static LogData data; // Static to keep off the small task stack (~60B savings)
    uint32_t lastSyncTime = HAL_GetTick();
    bool fileIsOpen = false;
    
    while (1) {
        // Wait forever for new data
        if (osMessageQueueGet(_logQueue, &data, NULL, osWaitForever) == osOK) {
            if (isMounted) {
                isSdCardBusy = true; // Prevent LCD updates during heavy SPI power draw
                
                if (!fileIsOpen) {
                    fr = f_open(&_file, "trip_log.csv", FA_OPEN_ALWAYS | FA_WRITE);
                    if (fr == FR_OK) {
                        f_lseek(&_file, f_size(&_file));
                        if (!headerWritten && f_size(&_file) == 0) {
                            const char* header = "Lat,Lon,Speed(km/h),Alt(m),Satellites,AccelX(g),AccelY(g),AccelZ(g),GyroX(dps),GyroY(dps),GyroZ(dps)\n";
                            UINT bw;
                            f_write(&_file, header, strlen(header), &bw);
                            headerWritten = true;
                        }
                        fileIsOpen = true;
                    }
                }
                
                if (fileIsOpen) {
                    // Static buffer + vars to keep stack usage minimal
                    static char buffer[160];
                    static int lat_i, lat_f, lon_i, lon_f, spd_i, spd_f, alt_i, alt_f;
                    static int ax, ay, az, gx, gy, gz;
                    lat_i = (int)data.gps.latitude;
                    lat_f = (int)((data.gps.latitude - lat_i) * 1000000);
                    if (lat_f < 0) lat_f = -lat_f;
                    lon_i = (int)data.gps.longitude;
                    lon_f = (int)((data.gps.longitude - lon_i) * 1000000);
                    if (lon_f < 0) lon_f = -lon_f;
                    spd_i = (int)data.gps.speedKmh;
                    spd_f = (int)((data.gps.speedKmh - spd_i) * 10);
                    if (spd_f < 0) spd_f = -spd_f;
                    alt_i = (int)data.gps.altitude;
                    alt_f = (int)((data.gps.altitude - alt_i) * 10);
                    if (alt_f < 0) alt_f = -alt_f;
                    ax = (int)(data.imu.accelX * 100);
                    ay = (int)(data.imu.accelY * 100);
                    az = (int)(data.imu.accelZ * 100);
                    gx = (int)(data.imu.gyroX * 100);
                    gy = (int)(data.imu.gyroY * 100);
                    gz = (int)(data.imu.gyroZ * 100);
                    
                    snprintf(buffer, sizeof(buffer), 
                             "%d.%06d,%d.%06d,%d.%d,%d.%d,%d,%d.%02d,%d.%02d,%d.%02d,%d.%02d,%d.%02d,%d.%02d\n",
                             lat_i, lat_f, lon_i, lon_f,
                             spd_i, spd_f, alt_i, alt_f,
                             (int)data.gps.satellites,
                             ax/100, (ax<0?-ax:ax)%100, ay/100, (ay<0?-ay:ay)%100, az/100, (az<0?-az:az)%100,
                             gx/100, (gx<0?-gx:gx)%100, gy/100, (gy<0?-gy:gy)%100, gz/100, (gz<0?-gz:gz)%100);
                    
                    UINT bytesWritten;
                    f_write(&_file, buffer, strlen(buffer), &bytesWritten);
                    
                    // Sync the file to SD card every 3 seconds to prevent data loss, instead of every message
                    uint32_t currentTick = HAL_GetTick();
                    if (currentTick - lastSyncTime >= 3000) {
                        f_sync(&_file);
                        lastSyncTime = currentTick;
                    }
                } else {
                    // File failed to open - try to remount
                    f_mount(NULL, "", 0);
                    sd_mount_res = f_mount(&_fs, "", 1);
                    isMounted = (sd_mount_res == FR_OK);
                }
                isSdCardBusy = false;
            } else {
                // Keep trying to mount if it failed originally
                // NOTE: Do NOT set isSdCardBusy here - mount retries don't draw
                // enough power to cause voltage sag, and blocking LCD for 2-3 seconds
                // during timeout makes the screen unresponsive.
                vTaskDelay(pdMS_TO_TICKS(2000));
                sd_mount_res = f_mount(&_fs, "", 1);
                isMounted = (sd_mount_res == FR_OK);
                if (isMounted) {
                    snprintf((char*)sd_test_read_buffer, sizeof(sd_test_read_buffer), "remount OK");
                }
            }
        }
    }
}
