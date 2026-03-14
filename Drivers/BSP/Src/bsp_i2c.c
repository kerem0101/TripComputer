#include "bsp_i2c.h"
#include "main.h"

// Defined in main.c or i2c.c by CubeMX
extern I2C_HandleTypeDef hi2c1; 

void bsp_i2c_init(void) {
    // CubeMX handles MX_I2C1_Init(), but you can add custom resets here
}

bool bsp_i2c_read_mem(uint8_t dev_addr, uint16_t mem_addr, uint8_t* data, uint16_t len) {
    // HAL_I2C_Mem_Read blocks until data is received. 
    // Timeout is set to 10ms.
    HAL_StatusTypeDef status = HAL_I2C_Mem_Read(&hi2c1, dev_addr, mem_addr, I2C_MEMADD_SIZE_8BIT, data, len, 10);
    return (status == HAL_OK);
}

bool bsp_i2c_write_mem(uint8_t dev_addr, uint16_t mem_addr, uint8_t* data, uint16_t len) {
    HAL_StatusTypeDef status = HAL_I2C_Mem_Write(&hi2c1, dev_addr, mem_addr, I2C_MEMADD_SIZE_8BIT, data, len, 10);
    return (status == HAL_OK);
}