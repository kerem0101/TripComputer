#pragma once
#include <cstdint>

class IDisplay {
public:
    virtual ~IDisplay() {}

    // Start display initialization 
    virtual void init() = 0;

    // Clear the display content
    virtual void clear() = 0;

    // Write text to the display at specified position (row, col)
    // Exp: print(0, 0, "Speed: 50 km/h");
    virtual void print(uint8_t row, uint8_t col, const char* text) = 0;

    // Backlight control: turn on/off the display backlight
    virtual void setBacklight(bool state) = 0;
};