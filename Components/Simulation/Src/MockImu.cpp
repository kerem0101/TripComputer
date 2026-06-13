#include "MockImu.hpp"

bool MockImu::init() {
    _data.accelX = 0.0f;
    _data.accelY = 0.0f;
    _data.accelZ = 1.0f; // Gravity
    _data.gyroX = 0.0f;
    _data.gyroY = 0.0f;
    _data.gyroZ = 0.0f;
    _data.tempC = 25.0f;
    _data.isValid = true;
    _lastUpdateTick = bsp_system_get_tick_ms();
    return true;
}

bool MockImu::update() {
    uint32_t currentTick = bsp_system_get_tick_ms();
    if (currentTick - _lastUpdateTick >= 50) { // IMU typically updates faster, e.g. 20Hz
        _lastUpdateTick = currentTick;
        // Just add some slight fake vibration
        static float vibration = 0.01f;
        _data.accelZ = 1.0f + vibration;
        vibration = -vibration;
        return true;
    }
    return false;
}

ImuData MockImu::getData() const {
    return _data;
}
