#pragma once
#include "IState.hpp"
#include "IDisplay.hpp"
#include "IGps.hpp"
#include "Button.hpp"

class StateManager {
public:
    // Constructor: Takes display and GPS references
    StateManager(IDisplay& display, IGps& gps);

    // Set initial state without triggering drawing (useful before RTOS starts)
    void setInitialState(IState* initialState);

    // Transitions to a new state (screen)
    void changeState(IState* newState);

    // Force redraw of the current state (useful after LCD init)
    void forceRedraw();

    // Triggers periodic updates for the active state
    void update();

    // Forwards incoming button events to the active state
    void handleButtonEvent(ButtonEvent event);

    // Provide access to the GPS driver for resetting trip distance
    IGps& getGps() { return _gps; }

private:
    IDisplay& _display;
    IGps& _gps;
    IState* _currentState;
};