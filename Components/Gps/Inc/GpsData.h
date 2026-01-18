#pragma once
#include <cstdint>
// Structure holding raw data received from the GPS
struct GpsData {
    float latitude;    // Latitude
    float longitude;   // Longitude
    float speedKmh;    // Speed (km/h)
    float altitude;    // Altitude
    uint8_t satellites;// Number of satellites
    bool isValid;      // Is the data valid? (GPS fix)
};