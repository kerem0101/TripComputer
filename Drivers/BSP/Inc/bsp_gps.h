#pragma once
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// GPS RX buffer size (enough for ~2 NMEA sentences)
#define GPS_RX_BUFFER_SIZE 512

// Initialization function
void bsp_gps_dma_init(void);

// Function called by C++ side to read data
// buffer: destination for copied data
// max_len: maximum number of bytes to read
// return: number of bytes read
uint16_t bsp_gps_read_data(uint8_t* buffer, uint16_t max_len);

#ifdef __cplusplus
}
#endif
