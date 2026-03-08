#pragma once
#include "IState.hpp"

class SpeedState : public IState {
public:
    void onEnter(IDisplay& display) override;
    void onUpdate(IDisplay& display, IGps& gps) override;
    void onButtonEvent(ButtonEvent event, StateManager& manager) override;
    void onExit() override;
};