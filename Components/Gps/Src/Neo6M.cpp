#include "Neo6M.hpp"
#include "bsp_gps.h"
#include <cstring>
#include <cstdio>
#include "NmeaParser.hpp"

// Constructor implementation
Neo6M::Neo6M(){
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
    NmeaParser _parser;

    bool newDataAvailable = false;

        // loop for each received byte
        for (uint16_t i = 0; i < len; i++) {
            
            // every byte is processed
            if (_parser.processByte((char)tempBuffer[i], _data)) {
                
                // processByte returned true -> new valid data available
                newDataAvailable = true;
            }
        }
        
    return newDataAvailable;
}

// Data read function
GpsData Neo6M::getData() const {
    return _data;
}