#include "DisplayTask.hpp"
#include "bsp_board.h" // for bsp_system_get_tick_ms()
#include <cstdio> // for sprintf 

DisplayTask::DisplayTask(StateManager& stateManager, Button& mainButton, IDisplay& display) 
    : Thread("DisplayTask", 256, 2), // Stack: 256, Priority: Normal
      _stateManager(stateManager),
      _mainButton(mainButton),
      _display(display) {
}

void DisplayTask::run() {
    uint32_t lastDisplayUpdateMs = 0;
    const uint32_t displayUpdateIntervalMs = 500; // Update LCD every 500ms to avoid flicker

    _display.init(); // Initialize the display once at the start
    _display.clear(); // Clear the display before starting the main loop

    while (1) {
        uint32_t currentTick = bsp_system_get_tick_ms();

        // 1. FAST POLLING: Update the button state (Debounce logic)
        _mainButton.update(currentTick);
        ButtonEvent event = _mainButton.getEvent();

        // 2. EVENT HANDLING: If a button was pressed, forward it immediately
        if (event != ButtonEvent::None) {
            _stateManager.handleButtonEvent(event);
            
            // Optional: Force an immediate display update when a button is pressed
            // so the user doesn't feel a lag when changing screens.
            _stateManager.update();
            lastDisplayUpdateMs = currentTick; 
        }

        // 3. SLOW POLLING: Update the display periodically for GPS data
        if ((currentTick - lastDisplayUpdateMs) >= displayUpdateIntervalMs) {
            _stateManager.update();
            lastDisplayUpdateMs = currentTick;
        }

        // Sleep for 20ms. This yields the CPU to other tasks (like GPS) 
        // and defines our button sampling rate.
        delay(20); 
    }
}