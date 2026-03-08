#include "bsp_button.h"
#include "main.h" 

bool bsp_button_is_pressed(bsp_button_id_t btn_id) {
    if (btn_id == BTN_ID_MAIN) {
        // Pull-Up button: Pressed = LOW (GPIO_PIN_RESET), Released = HIGH (GPIO_PIN_SET)
        return HAL_GPIO_ReadPin(BTN_MAIN_GPIO_Port, BTN_MAIN_Pin) == GPIO_PIN_RESET;
    }
    return false;
}