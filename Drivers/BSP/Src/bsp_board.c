#include "bsp_board.h"
#include "main.h"      // CubeMX pin definitions and HAL library from here
// or #include "stm32f4xx_hal.h"

void bsp_init(void) {
    // Board specific initializations can be added here
    // For now, we assume CubeMX has initialized GPIOs
}

void bsp_led_on(Board_Led_t led) {
    if (led == LED_SYSTEM_STATUS) {
        // Note: Adjust GPIO port and pin according to your board configuration
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
    }
}

void bsp_led_off(Board_Led_t led) {
    if (led == LED_SYSTEM_STATUS) {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
    }
}

void bsp_led_toggle(Board_Led_t led) {
    if (led == LED_SYSTEM_STATUS) {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
    }
}

void bsp_delay(uint32_t ms) {
    HAL_Delay(ms); // STM32 HAL function for delay
}