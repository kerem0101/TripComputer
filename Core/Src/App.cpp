#include "App.h"
#include "main.h" // for HAL library access
#include "bsp_board.h" // for BSP functions
#include "SystemTask.hpp" // for SystemTask class

SystemTask systemTask;

void App_Main() {
    
    
    bool status = systemTask.start();

    if (!status) {
        // Handle task creation failure (optional)
        while (1) {
            // Stay here if task creation failed

        }
    }
}