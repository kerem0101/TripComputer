#pragma once
#include "IDisplay.hpp"
#include <cstdint> // uint8_t type for row and column parameters

class LcdParallel : public IDisplay {
public:
    LcdParallel();

    void init() override;
    void clear() override;
    void print(uint8_t row, uint8_t col, const char* text) override;
    void setBacklight(bool state) override;

private:
    void sendCommand(uint8_t cmd);
    void sendData(uint8_t data);
    void sendByte(uint8_t val);
    void writeNibble(uint8_t nibble);
};