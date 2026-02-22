#pragma once
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// BSP functions for LCD control (4-bit mode), called from C++ code
void bsp_lcd_set_rs(bool state);
void bsp_lcd_set_en(bool state);
void bsp_lcd_write_data_pins(uint8_t nibble); // D4-D7 setting (4-bit mode)

// Delay functions for LCD timing 
void bsp_lcd_delay_ms(uint32_t ms);
void bsp_lcd_delay_us(uint32_t us);

#ifdef __cplusplus
}
#endif