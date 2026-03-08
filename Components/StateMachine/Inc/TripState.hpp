#pragma once
#include "IState.hpp"
#include <cstdint>

class TripState : public IState {
public:
    TripState(); // Constructor to initialize variables

    void onEnter(IDisplay& display) override;
    void onUpdate(IDisplay& display, IGps& gps) override;
    void onButtonEvent(ButtonEvent event, StateManager& manager) override;
    void onExit() override;

private:
    float _tripDistanceKm;      // Stores the accumulated distance in kilometers
    uint32_t _lastUpdateTimeMs; // Used for time delta (dt) calculation
};