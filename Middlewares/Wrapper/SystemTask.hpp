#pragma once
#include "Thread.hpp"
#include "bsp_board.h" 

class SystemTask : public Thread {
public:
    // Priority: low priority (tskIDLE_PRIORITY + 1)
    SystemTask() : Thread("SystemBlink", 128, tskIDLE_PRIORITY + 1) {}

protected:
    // Override the run method
    void run() override {
        // Individual task setup (if needed)
        
        while (1) {
            // BSP LED toggle function
            BSP_LED_Toggle(LED_SYSTEM_STATUS);
            
            // Delay for 500 milliseconds
            delay(500);
        }
    }
};