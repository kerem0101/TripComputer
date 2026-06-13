#include "MockGps.hpp"

void MockGps::init() {
    _data.latitude = 41.0082f; // Istanbul
    _data.longitude = 28.9784f;
    _data.speedKmh = 0.0f;
    _data.altitude = 100.0f;
    _data.satellites = 8;
    _data.isValid = true;
    _lastUpdateTick = bsp_system_get_tick_ms();
    _targetSpeed = 100.0f;
}

bool MockGps::update() {
    uint32_t currentTick = bsp_system_get_tick_ms();
    if (currentTick - _lastUpdateTick >= 1000) { // Update once per second
        _lastUpdateTick = currentTick;
        
        // Increase speed step by step
        if (_data.speedKmh < _targetSpeed) {
            _data.speedKmh += 2.5f; // accelerate gradually
        } else {
            _data.speedKmh = 0.0f; // Reset to 0 to loop the scenario
        }
        
        // Simulate movement in latitude (approximate)
        // 1 degree lat is ~111km. Speed is in km/h. Distance in 1 sec = speed / 3600 km.
        float distKm = _data.speedKmh / 3600.0f;
        _data.latitude += (distKm / 111.0f);
        
        return true;
    }
    return false;
}

GpsData MockGps::getData() const {
    return _data;
}

void MockGps::resetTripDistance() {
    _data.tripDistanceKm = 0.0f;
}
