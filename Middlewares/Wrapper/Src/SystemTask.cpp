#include "SystemTask.hpp"
#include "bsp_board.h" 

// Priority: low priority (tskIDLE_PRIORITY + 1)
SystemTask::SystemTask() : Thread("SystemBlink", 128, tskIDLE_PRIORITY + 1) {}


// Override the run method
void SystemTask::run() {
    // Individual task setup (if needed)
    
    while (1) {
        // BSP LED toggle function
        bsp_led_toggle(LED_SYSTEM_STATUS);
        
        // Delay for 500 milliseconds
        bsp_delay(500);
    }
}
