#pragma once
#include "Button.hpp"
#include "IDisplay.hpp"
#include "IGps.hpp"

// In the future, we might want to add more parameters to the state methods, such as sensor data, time information, etc. For now, we'll keep it simple.
class StateManager;

class IState {
public:
    virtual ~IState() {}

    // First time the state becomes active, this method is called once (Initialization)
    virtual void onEnter(IDisplay& display) = 0;

    // Periodically called while the state is active (Main logic, Display updates, etc.)
    virtual void onUpdate(IDisplay& display, IGps& gps) = 0;

    // When a button event occurs, this method is called (Button handling)
    virtual void onButtonEvent(ButtonEvent event, StateManager& manager) = 0;

    // Executed when the state is about to be replaced by another state (Cleanup)
    virtual void onExit() = 0;
};