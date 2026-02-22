#pragma once
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// LED definitions
typedef enum {
    LED_SYSTEM_STATUS, // onboard LED
    // feature additions: LED_ERROR, LED_TX, etc.
} Board_Led_t;

// --- Function Prototypes ---

// Starstup and Initialization
void bsp_init(void);

// LED Control
void bsp_led_on(Board_Led_t led);
void bsp_led_off(Board_Led_t led);
void bsp_led_toggle(Board_Led_t led);

// Timing (to avoid direct use of HAL_Delay)
void bsp_delay(uint32_t ms);

#ifdef __cplusplus
}
#endif
