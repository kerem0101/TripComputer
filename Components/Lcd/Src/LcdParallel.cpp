#include "LcdParallel.hpp"
#include "bsp_lcd.h" // LCD pin control functions from BSP layer

LcdParallel::LcdParallel() {
    // Constructor can be empty since pin control is handled by BSP functions
}

void LcdParallel::init() {
    bsp_lcd_delay_ms(50); // Wait for LCD to power up (typical 40-50 ms)
    
    // --- 4-Bit Starting ---
    writeNibble(0x03); bsp_lcd_delay_ms(5);
    writeNibble(0x03); bsp_lcd_delay_ms(1);
    writeNibble(0x03); bsp_lcd_delay_ms(1);
    
    writeNibble(0x02); // Switch 4-Bit mode 
    
    sendCommand(0x28); // 4-bit, 2 row, 5x8 font
    sendCommand(0x0C); // Display open, cursor closed
    sendCommand(0x06); // Write and move cursor right
    clear();
}

void LcdParallel::clear() {
    sendCommand(0x01);
    bsp_lcd_delay_ms(2); // Clear command takes longer time
}

void LcdParallel::print(uint8_t row, uint8_t col, const char* text) {
    uint8_t address = (row == 0) ? 0x80 : 0xC0;
    address += col;
    sendCommand(address);

    while (*text) {
        sendData(*text++);
    }
}

void LcdParallel::setBacklight(bool state) {
    // Backlight control can be implemented here if your LCD supports it via a GPIO pin.
    // For example, if backlight is connected to a pin, you can use HAL_GPIO_WritePin to control it. (Not implemented here as it depends on your specific hardware setup)
}

void LcdParallel::sendCommand(uint8_t cmd) {
    bsp_lcd_set_rs(false); // RS = 0 command
    sendByte(cmd);
}

void LcdParallel::sendData(uint8_t data) {
    bsp_lcd_set_rs(true);  // RS = 1 data
    sendByte(data);
}

void LcdParallel::sendByte(uint8_t val) {
    writeNibble(val >> 4);   // First send the high nibble (upper 4 bits)
    writeNibble(val & 0x0F); // Then send the low nibble (lower 4 bits)
}

void LcdParallel::writeNibble(uint8_t nibble) {
    bsp_lcd_write_data_pins(nibble); // Put the nibble on the data pins D4-D7
    
    // Enable Pulse (E pin trigger)
    bsp_lcd_set_en(true);
    bsp_lcd_delay_us(10); 
    bsp_lcd_set_en(false);
    bsp_lcd_delay_us(50); // small delay for command to process (can be optimized based on command type)
}