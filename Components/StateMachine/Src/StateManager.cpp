#include "StateManager.hpp"

StateManager::StateManager(IDisplay& display, IGps& gps) 
    : _display(display), _gps(gps), _currentState(nullptr) {
}

void StateManager::setInitialState(IState* initialState) {
    _currentState = initialState;
}

void StateManager::changeState(IState* newState) {
    if (newState != nullptr) {
        if (_currentState != nullptr) {
            _currentState->onExit();
        }
        _currentState = newState;
        _display.clear();
        _currentState->onEnter(_display);
    }
}

void StateManager::forceRedraw() {
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