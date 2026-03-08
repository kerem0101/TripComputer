#pragma once
#include "IState.hpp"
#include "IDisplay.hpp"
#include "IGps.hpp"
#include "Button.hpp"

class StateManager {
public:
    // Constructor: Takes display and GPS references
    StateManager(IDisplay& display, IGps& gps);

    // Transitions to a new state (screen)
    void changeState(IState* newState);

    // Triggers periodic updates for the active state
    void update();

    // Forwards incoming button events to the active state
    void handleButtonEvent(ButtonEvent event);

private:
    IDisplay& _display;
    IGps& _gps;
    IState* _currentState;
};