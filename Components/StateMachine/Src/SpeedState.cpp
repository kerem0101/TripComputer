#include "SpeedState.hpp"
#include "StateManager.hpp" // Required for state transition operations
#include <cstdio>           // Required for sprintf

// Forward declaration of the next state (e.g., Trip State) from the global scope.
// (As the application grows, these will be linked inside App.cpp)
extern IState* tripStateInstance; 

void SpeedState::onEnter(IDisplay& display) {
    // Print the screen title only once upon entry to prevent flickering
    display.print(0, 0, "  ---SPEED---   ");
}

void SpeedState::onUpdate(IDisplay& display, IGps& gps) {
    GpsData data = gps.getData();
    char buffer[17];

    if (data.isValid) {
        sprintf(buffer, "%5d km/h  ", (int)data.speedKmh);
    } else {
        sprintf(buffer, "No Signal!      ");
    }
    
    display.print(1, 0, buffer);
}

void SpeedState::onButtonEvent(ButtonEvent event, StateManager& manager) {
    // Single-button architecture: Move to the next state on a short click
    if (event == ButtonEvent::ShortClick) {
        if (tripStateInstance != nullptr) {
            manager.changeState(tripStateInstance);
        }
    }
    // Nothing to do on long press for the current speed screen
}

void SpeedState::onExit() {
    // No specific variables to delete/reset upon exit
}