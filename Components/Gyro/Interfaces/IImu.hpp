#pragma once
#include <cstdint>

// Structure to hold 6-DOF (Degrees of Freedom) data
struct ImuData {
    float accelX; // Acceleration in g (gravity)
    float accelY;
    float accelZ;
    
    float gyroX;  // Angular velocity in degrees per second (dps)
    float gyroY;
    float gyroZ;
    
    float tempC;  // Internal temperature in Celsius
    
    bool isValid; // True if the sensor is communicating properly
};

class IImu {
public:
    virtual ~IImu() {}

    // Initialize the sensor (wake up, configure ranges)
    virtual bool init() = 0;

    // Read the latest raw data and convert to human-readable units
    virtual bool update() = 0;

    // Get the cached data
    virtual ImuData getData() const = 0;
};