#include "StateManager.hpp"

StateManager::StateManager(IDisplay& display, IGps& gps) 
    : _display(display), _gps(gps), _currentState(nullptr) {
}

void StateManager::changeState(IState* newState) {
    // Perform exit routines for the previous state
    if (_currentState != nullptr) {
        _currentState->onExit();
    }

    _currentState = newState;
    
    // Clear the hardware display when transitioning to a new state
    _display.clear(); 

    // Perform entry (setup) routines for the new state
    if (_currentState != nullptr) {
        _currentState->onEnter(_display);
    }
}

void StateManager::update() {
    // Call the update function of the currently active state (Polymorphism)
    if (_currentState != nullptr) {
        _currentState->onUpdate(_display, _gps);
    }
}

void StateManager::handleButtonEvent(ButtonEvent event) {
    // If a button event occurred and there is an active state, forward the event
    if (_currentState != nullptr && event != ButtonEvent::None) {
        _currentState->onButtonEvent(event, *this);
    }
}