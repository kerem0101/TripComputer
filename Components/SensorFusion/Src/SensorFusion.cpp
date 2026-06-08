#include "SensorFusion.hpp"
#include "main.h" // For HAL_GetTick()

SensorFusion::SensorFusion(IGps& gps, IImu& imu)
    : _gps(gps), _imu(imu), _lastUpdateMs(0) {
    // Initialize fused data structure
    _fusedData = {0};
}

void SensorFusion::init() {
    _gps.init();
    _imu.init();
    _kf.init(0.0f, 0.0f);
    _lastUpdateMs = HAL_GetTick();
}

bool SensorFusion::update() {
    bool newDataAvailable = false;
    uint32_t currentMs = HAL_GetTick();
    float dt = (currentMs - _lastUpdateMs) / 1000.0f; // dt in seconds
    
    // Prevent dt from being zero or too large
    if (dt <= 0.0f) {
        return false;
    }
    if (dt > 0.1f) {
        dt = 0.1f; // Cap at 100ms
    }
    
    _lastUpdateMs = currentMs;

    // 1. Read IMU data and perform predict step
    if (_imu.update()) {
        ImuData imuData = _imu.getData();
        if (imuData.isValid) {
            // ========================================================
            // AXIS SELECTION
            // Change the forwardAccel variable below to match your PCB 
            // mounting orientation. 
            // If forward is X-axis: forwardAccel = imuData.accelX;
            // If forward is Y-axis: forwardAccel = imuData.accelY;
            // ========================================================
            float forwardAccel = imuData.accelY; 
            
            // Convert 'g' to m/s^2 (1g = 9.80665 m/s^2)
            float accel_ms2 = forwardAccel * 9.80665f;
            
            _kf.predict(accel_ms2, dt);
            newDataAvailable = true;
        }
    }

    // 2. Read GPS data and perform update step
    if (_gps.update()) {
        GpsData gpsData = _gps.getData();
        if (gpsData.isValid) {
            // GPS speed is usually in km/h, convert to m/s for the Kalman filter
            float measured_v_ms = gpsData.speedKmh / 3.6f;
            
            _kf.update(measured_v_ms);
            
            // We copy all absolute position/satellite data from the real GPS
            _fusedData = gpsData;
            newDataAvailable = true;
        }
    }

    // 3. Update the fused speed (convert back to km/h for the UI)
    float fusedVelocity_ms = _kf.getVelocity();
    
    // Prevent negative speeds (reverse) if not expected, though Kalman might dip slightly negative
    if (fusedVelocity_ms < 0.0f) fusedVelocity_ms = 0.0f;
    
    _fusedData.speedKmh = fusedVelocity_ms * 3.6f;
    
    return newDataAvailable;
}

GpsData SensorFusion::getData() const {
    return _fusedData;
}
