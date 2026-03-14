#pragma once
#include "IImu.hpp"

// Default I2C address for MPU6050 (AD0 pin connected to GND)
// Note: STM32 HAL requires the 8-bit shifted address, so 0x68 << 1 = 0xD0
#define MPU6050_ADDR 0xD0

class Mpu6050 : public IImu {
public:
    Mpu6050();

    bool init() override;
    bool update() override;
    ImuData getData() const override;

private:
    ImuData _data;

    // MPU6050 internal register addresses
    static const uint16_t REG_SMPLRT_DIV = 0x19;
    static const uint16_t REG_GYRO_CONFIG = 0x1B;
    static const uint16_t REG_ACCEL_CONFIG = 0x1C;
    static const uint16_t REG_ACCEL_XOUT_H = 0x3B;
    static const uint16_t REG_PWR_MGMT_1 = 0x6B;
    static const uint16_t REG_WHO_AM_I = 0x75;

    // Sensitivities for default settings (+-2g, +-250dps)
    static constexpr float ACCEL_SCALE = 16384.0f;
    static constexpr float GYRO_SCALE = 131.0f;
};