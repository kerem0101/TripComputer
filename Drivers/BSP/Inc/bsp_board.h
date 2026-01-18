#ifndef BSP_BOARD_H
#define BSP_BOARD_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

// LED definitions
typedef enum {
    LED_SYSTEM_STATUS, // onboard LED
    // feature additions: LED_ERROR, LED_TX, etc.
} Board_Led_t;

// --- Function Prototypes ---

// Starstup and Initialization
void BSP_Init(void);

// LED Control
void BSP_LED_On(Board_Led_t led);
void BSP_LED_Off(Board_Led_t led);
void BSP_LED_Toggle(Board_Led_t led);

// Timing (to avoid direct use of HAL_Delay)
void BSP_Delay(uint32_t ms);

#ifdef __cplusplus
}
#endif

#endif // BSP_BOARD_H