#include "bsp_board.h"
#include "main.h"      // CubeMX pin definitions and HAL library from here
// or #include "stm32f4xx_hal.h"

void BSP_Init(void) {
    // Board specific initializations can be added here
    // For now, we assume CubeMX has initialized GPIOs
}

void BSP_LED_On(Board_Led_t led) {
    if (led == LED_SYSTEM_STATUS) {
        // Note: Adjust GPIO port and pin according to your board configuration
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
    }
}

void BSP_LED_Off(Board_Led_t led) {
    if (led == LED_SYSTEM_STATUS) {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
    }
}

void BSP_LED_Toggle(Board_Led_t led) {
    if (led == LED_SYSTEM_STATUS) {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
    }
}

void BSP_Delay(uint32_t ms) {
    HAL_Delay(ms); // STM32 HAL function for delay
}