#pragma once

#include "Thread.hpp"
#include "IDisplay.hpp"
#include "StateManager.hpp"
#include "Button.hpp"

class DisplayTask : public Thread {
public:
    // Constructor: Takes the StateManager and the main Button as dependencies
    DisplayTask(StateManager& stateManager, Button& mainButton, IDisplay& display);

protected:
    void run() override;

private:
    StateManager& _stateManager;
    Button& _mainButton;
    IDisplay& _display;
};