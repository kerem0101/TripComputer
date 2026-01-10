#include "App.h"
#include "main.h" // for HAL library access
#include "bsp_board.h"

void App_Main() {
    while(1) {
        
        BSP_LED_Toggle(LED_SYSTEM_STATUS);
        BSP_Delay(500);
    }
}