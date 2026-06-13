#include "LoggerTask.hpp"
#include <stdio.h>
#include <string.h>

LoggerTask::LoggerTask() : Thread("LoggerTask", 512, 1) { // Normal priority, 512 words stack (FatFs needs memory)
    _logQueue = NULL;
}

void LoggerTask::init() {
    // Create a queue that can hold up to 10 LogData items
    _logQueue = osMessageQueueNew(10, sizeof(LogData), NULL);
}

bool LoggerTask::pushData(const LogData& data) {
    if (_logQueue == NULL) return false;
    // Push with 0 timeout (non-blocking for NavigationTask)
    return (osMessageQueuePut(_logQueue, &data, 0, 0) == osOK);
}

void LoggerTask::run() {
    // 1. Mount the SD card File System
    FRESULT fr = f_mount(&_fs, "", 1); // 1 = mount immediately
    bool isMounted = (fr == FR_OK);

    // Write CSV header if file doesn't exist
    if (isMounted) {
        FIL fil;
        fr = f_open(&fil, "trip_log.csv", FA_OPEN_ALWAYS | FA_WRITE);
        if (fr == FR_OK) {
            // Move pointer to end of file to append
            f_lseek(&fil, f_size(&fil));
            // Check if file is empty by getting size
            if (f_size(&fil) == 0) {
                const char* header = "Lat,Lon,Speed(km/h),Alt(m),Satellites,AccelX(g),AccelY(g),AccelZ(g),GyroX(dps),GyroY(dps),GyroZ(dps)\n";
                UINT bw;
                f_write(&fil, header, strlen(header), &bw);
            }
            f_close(&fil);
        }
    }

    LogData data;
    while (1) {
        // Wait forever for new data
        if (osMessageQueueGet(_logQueue, &data, NULL, osWaitForever) == osOK) {
            if (isMounted) {
                FIL fil;
                fr = f_open(&fil, "trip_log.csv", FA_OPEN_ALWAYS | FA_WRITE);
                if (fr == FR_OK) {
                    f_lseek(&fil, f_size(&fil));
                    char buffer[128];
                    // Format: Lat,Lon,Speed,Alt,Sat,Ax,Ay,Az,Gx,Gy,Gz
                    snprintf(buffer, sizeof(buffer), "%.6f,%.6f,%.2f,%.1f,%u,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\n",
                             data.gps.latitude,
                             data.gps.longitude,
                             data.gps.speedKmh,
                             data.gps.altitude,
                             data.gps.satellites,
                             data.imu.accelX,
                             data.imu.accelY,
                             data.imu.accelZ,
                             data.imu.gyroX,
                             data.imu.gyroY,
                             data.imu.gyroZ);
                    
                    UINT bytesWritten;
                    f_write(&fil, buffer, strlen(buffer), &bytesWritten);
                    
                    // Close the file to ensure data is saved immediately
                    f_close(&fil);
                } else {
                    // Try to remount if there's an error (e.g., SD card was removed and reinserted)
                    f_mount(NULL, "", 0); // unmount
                    isMounted = (f_mount(&_fs, "", 1) == FR_OK);
                }
            } else {
                // Keep trying to mount if it failed originally
                isMounted = (f_mount(&_fs, "", 1) == FR_OK);
            }
        }
    }
}
