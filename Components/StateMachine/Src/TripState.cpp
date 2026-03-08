#include "TripState.hpp"
#include "StateManager.hpp"
#include "bsp_board.h" // For time functions
#include <cstdio>   // Required for sprintf


// Forward declaration to loop back to the Speed screen
extern IState* speedStateInstance; 

TripState::TripState() : _tripDistanceKm(0.0f), _lastUpdateTimeMs(0) {
}

void TripState::onEnter(IDisplay& display) {
    // Print the screen title
    display.print(0, 0, "  ---TRIP A---  ");
    
    // Reset the timer using the BSP layer!
    _lastUpdateTimeMs = bsp_system_get_tick_ms();
}

void TripState::onUpdate(IDisplay& display, IGps& gps) {
    GpsData data = gps.getData();
    
    // Get time from BSP layer
    uint32_t currentTick = bsp_system_get_tick_ms();
    uint32_t dtMs = currentTick - _lastUpdateTimeMs;
    _lastUpdateTimeMs = currentTick;

    // Simple Integration: Distance = Speed * Time
    if (data.isValid) {
        // Convert dt from milliseconds to hours
        float dtHours = (float)dtMs / 3600000.0f;
        
        // Accumulate distance
        _tripDistanceKm += (data.speedKmh * dtHours);
    }

    // Format and display the distance
    char buffer[17];
    sprintf(buffer, "%5.2f km       ", _tripDistanceKm);
    display.print(1, 0, buffer);
}

void TripState::onButtonEvent(ButtonEvent event, StateManager& manager) {
    if (event == ButtonEvent::ShortClick) {
        if (speedStateInstance != nullptr) {
            manager.changeState(speedStateInstance);
        }
    } 
    else if (event == ButtonEvent::LongPress) {
        _tripDistanceKm = 0.0f;
    }
}

void TripState::onExit() {
}