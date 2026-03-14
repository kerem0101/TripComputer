#pragma once
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// Initialize I2C peripherals (optional, usually done by CubeMX)
void bsp_i2c_init(void);

// Read memory registers from an I2C device
// dev_addr: 8-bit device address
// mem_addr: Internal register address
// data: Buffer to store read bytes
// len: Number of bytes to read
bool bsp_i2c_read_mem(uint8_t dev_addr, uint16_t mem_addr, uint8_t* data, uint16_t len);

// Write to memory registers of an I2C device
bool bsp_i2c_write_mem(uint8_t dev_addr, uint16_t mem_addr, uint8_t* data, uint16_t len);

#ifdef __cplusplus
}
#endif
