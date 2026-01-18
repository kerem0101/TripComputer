#include "Neo6M.hpp"
#include "bsp_board.h" // UART and hardware functions
#include "bsp_gps.h"
#include <cstring>
#include <cstdio>

// Constructor implementation
Neo6M::Neo6M() : _isValid(false) {
    // Clear the data structure
    memset(&_data, 0, sizeof(_data));
}

// Init function implementation
void Neo6M::init() {
    // Call the UART init function from the BSP layer
    // Start reception (DMA or interrupt)
    bsp_gps_dma_init();
}

// Update function implementation
bool Neo6M::update() {
    // Read data to temp buffer from BSP layer
    uint8_t tempBuffer[128];
    uint16_t len = bsp_gps_read_data(tempBuffer, sizeof(tempBuffer) - 1);

    if (len > 0) {
            tempBuffer[len] = '\0'; // string terminator
            // For debugging: print received data
            
            // we would normally parse the NMEA sentences here
            // processNMEA((char*)tempBuffer);
            
            // for testing, just mark data as valid
            return true;
        }
    return false;
}

// Data read function
GpsData Neo6M::getData() const {
    return _data;
}

// Private parser function
void Neo6M::parseNMEA(char* nmeaSentence) {
    // Example sentence:
    // $GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A
    
    // Parse the incoming sentence and populate the _data struct.
    // Example (simplified):
    if (strstr(nmeaSentence, "$GPRMC")) {
        // Parse using sscanf or strtok...
        // _data.latitude = ...
        // _data.isValid = true;
    }
}