#pragma once
#include "IGps.hpp"
#include "bsp_board.h" // For bsp_system_get_tick_ms()

class MockGps : public IGps {
public:
    void init() override;
    bool update() override;
    GpsData getData() const override;
    void resetTripDistance() override;

private:
    GpsData _data;
    uint32_t _lastUpdateTick;
    float _targetSpeed;
};
