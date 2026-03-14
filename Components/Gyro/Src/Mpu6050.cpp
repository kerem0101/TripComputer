#include "Mpu6050.hpp"
#include "bsp_i2c.h" // For I2C read/write functions

Mpu6050::Mpu6050() {
    _data.isValid = false;
}

bool Mpu6050::init() {
    uint8_t check = 0;
    uint8_t data = 0;

    // 1. Check if the sensor is responding (WHO_AM_I register should return 0x68)
    if (!bsp_i2c_read_mem(MPU6050_ADDR, REG_WHO_AM_I, &check, 1)) {
        return false; // I2C communication failed
    }

    if (check == 0x68) {
        // 2. Wake up the sensor (write 0 to Power Management 1)
        data = 0x00;
        bsp_i2c_write_mem(MPU6050_ADDR, REG_PWR_MGMT_1, &data, 1);

        // 3. Set data rate to 1KHz (Sample Rate Divider = 0)
        data = 0x00;
        bsp_i2c_write_mem(MPU6050_ADDR, REG_SMPLRT_DIV, &data, 1);

        // 4. Set accelerometer configuration to +-2g
        data = 0x00;
        bsp_i2c_write_mem(MPU6050_ADDR, REG_ACCEL_CONFIG, &data, 1);

        // 5. Set gyro configuration to +-250 dps
        data = 0x00;
        bsp_i2c_write_mem(MPU6050_ADDR, REG_GYRO_CONFIG, &data, 1);

        _data.isValid = true;
        return true;
    }

    return false; // Incorrect device ID
}

bool Mpu6050::update() {
    if (!_data.isValid) return false;

    // MPU6050 stores Accel, Temp, and Gyro data sequentially in 14 bytes
    uint8_t rawData[14];

    // Read 14 bytes starting from ACCEL_XOUT_H
    if (bsp_i2c_read_mem(MPU6050_ADDR, REG_ACCEL_XOUT_H, rawData, 14)) {
        
        // Combine high and low bytes (16-bit 2's complement values)
        int16_t accelX_raw = (int16_t)(rawData[0] << 8 | rawData[1]);
        int16_t accelY_raw = (int16_t)(rawData[2] << 8 | rawData[3]);
        int16_t accelZ_raw = (int16_t)(rawData[4] << 8 | rawData[5]);
        
        int16_t temp_raw   = (int16_t)(rawData[6] << 8 | rawData[7]);
        
        int16_t gyroX_raw  = (int16_t)(rawData[8] << 8 | rawData[9]);
        int16_t gyroY_raw  = (int16_t)(rawData[10] << 8 | rawData[11]);
        int16_t gyroZ_raw  = (int16_t)(rawData[12] << 8 | rawData[13]);

        // Convert raw values to human-readable units (g and degrees/sec)
        _data.accelX = (float)accelX_raw / ACCEL_SCALE;
        _data.accelY = (float)accelY_raw / ACCEL_SCALE;
        _data.accelZ = (float)accelZ_raw / ACCEL_SCALE;

        _data.gyroX = (float)gyroX_raw / GYRO_SCALE;
        _data.gyroY = (float)gyroY_raw / GYRO_SCALE;
        _data.gyroZ = (float)gyroZ_raw / GYRO_SCALE;

        // Temperature formula according to datasheet
        _data.tempC = ((float)temp_raw / 340.0f) + 36.53f;

        return true;
    }
    
    _data.isValid = false; // I2C read failed, mark data as invalid
    return false;
}

ImuData Mpu6050::getData() const {
    return _data;
}