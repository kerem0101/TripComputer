#include "TripState.hpp"
#include "StateManager.hpp"
#include "bsp_board.h" // For time functions
#include <cstdio>   // Required for sprintf


// Forward declaration to loop back to the Speed screen
extern IState* speedStateInstance; 

TripState::TripState() {
}

void TripState::onEnter(IDisplay& display) {
    // Print the screen title
    display.print(0, 0, "  ---TRIP A---  ");
}

void TripState::onUpdate(IDisplay& display, IGps& gps) {
    GpsData data = gps.getData();
    float distance = data.tripDistanceKm;

    // Format and display the distance using integer math to save stack space
    int integerKm = (int)distance;
    int fractionalKm = (int)((distance - integerKm) * 100);
    if (fractionalKm < 0) fractionalKm = -fractionalKm; // Handle negative just in case

    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%3d.%02d km       ", integerKm, fractionalKm);
    display.print(1, 0, buffer);
}

void TripState::onButtonEvent(ButtonEvent event, StateManager& manager) {
    if (event == ButtonEvent::ShortClick) {
        if (speedStateInstance != nullptr) {
            manager.changeState(speedStateInstance);
        }
    } 
    else if (event == ButtonEvent::LongPress) {
        manager.getGps().resetTripDistance();
    }
}

void TripState::onExit() {
}