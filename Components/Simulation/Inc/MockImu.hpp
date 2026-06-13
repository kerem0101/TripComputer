#pragma once
#include "IImu.hpp"
#include "bsp_board.h"

class MockImu : public IImu {
public:
    bool init() override;
    bool update() override;
    ImuData getData() const override;

private:
    ImuData _data;
    uint32_t _lastUpdateTick;
};
