#pragma once
#include <cstdint>
#include "bsp_button.h"


// Button events definition
enum class ButtonEvent {
    None,       // No event
    Pressed,    // When the button is initially pressed down
    Released,   // When the button is released after being pressed
    ShortClick, // Short press (pressed and released within a short time)
    LongPress   // Long press (pressed and held for a duration longer than the longPressTime threshold)
};

class Button {
public:
    // Constructor (default debounce time: 50ms, default long press time: 1000ms)
    Button(bsp_button_id_t id, uint32_t debounceMs = 50, uint32_t longPressMs = 1000);

    // Button state update method, should be called periodically (e.g., in the main loop)
    void update(uint32_t currentTickMs);

    // Event getter, returns the last detected button event and resets it to None
    ButtonEvent getEvent();

private:
    bsp_button_id_t _id;
    uint32_t _debounceTime;
    uint32_t _longPressTime;

    bool _state;
    bool _lastState;
    uint32_t _lastDebounceTime;
    uint32_t _pressTimestamp;
    bool _longPressTriggered;
    
    ButtonEvent _lastEvent;
};