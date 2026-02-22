#include "bsp_lcd.h"
#include "main.h" // HAL library and GPIO definitions from CubeMX

void bsp_lcd_set_rs(bool state) {
    HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, state ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void bsp_lcd_set_en(bool state) {
    HAL_GPIO_WritePin(LCD_EN_GPIO_Port, LCD_EN_Pin, state ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void bsp_lcd_write_data_pins(uint8_t nibble) {
    // 4-bit mode: we only care about the lower 4 bits of the nibble
    HAL_GPIO_WritePin(LCD_D4_GPIO_Port, LCD_D4_Pin, (nibble & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_D5_GPIO_Port, LCD_D5_Pin, (nibble & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_D6_GPIO_Port, LCD_D6_Pin, (nibble & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_D7_GPIO_Port, LCD_D7_Pin, (nibble & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void bsp_lcd_delay_ms(uint32_t ms) {
    HAL_Delay(ms); // it converts to vTaskDelay internally, so it won't block other tasks in FreeRTOS
}

void bsp_lcd_delay_us(uint32_t us) {
    // Very short delay for LCD timing (enable pulse width, etc.)
    for(volatile uint32_t i = 0; i < (us * 8); i++); 
}