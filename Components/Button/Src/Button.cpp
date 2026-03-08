#include "Button.hpp"

// Constructor implementation
Button::Button(bsp_button_id_t id, uint32_t debounceMs, uint32_t longPressMs)
    : _id(id), 
      _debounceTime(debounceMs), 
      _longPressTime(longPressMs),
      _state(false), 
      _lastState(false), 
      _lastDebounceTime(0), 
      _pressTimestamp(0),
      _longPressTriggered(false), 
      _lastEvent(ButtonEvent::None) {
}

void Button::update(uint32_t currentTickMs) {
    // Physical button state reading from BSP
    bool reading = bsp_button_is_pressed(_id);

    // If the reading has changed from the last state, reset the debounce timer
    if (reading != _lastState) {
        _lastDebounceTime = currentTickMs;
    }

    // If the change duration exceeds the debounce threshold, the physical noise has settled
    if ((currentTickMs - _lastDebounceTime) > _debounceTime) {
        
        // If the button state has changed (after debounce), update the state and generate events
        if (reading != _state) {
            _state = reading;

            if (_state == true) { // Button PRESSED
                _pressTimestamp = currentTickMs;
                _longPressTriggered = false;
                _lastEvent = ButtonEvent::Pressed;
            } 
            else { // Button RELEASED
                uint32_t pressDuration = currentTickMs - _pressTimestamp;
                
                if (!_longPressTriggered && pressDuration > 0) {
                    _lastEvent = ButtonEvent::ShortClick; // Short press
                } else {
                    _lastEvent = ButtonEvent::Released; // Long press followed by release
                }
            }
        }
    }

    // Check for long press event while the button is still pressed
    if (_state == true && !_longPressTriggered) {
        if ((currentTickMs - _pressTimestamp) > _longPressTime) {
            _longPressTriggered = true;
            _lastEvent = ButtonEvent::LongPress;
        }
    }

    _lastState = reading;
}

ButtonEvent Button::getEvent() {
    ButtonEvent e = _lastEvent;
    _lastEvent = ButtonEvent::None; // Reset event after reading
    return e;
}