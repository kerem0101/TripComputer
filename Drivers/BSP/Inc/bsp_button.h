#pragma once
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    BTN_ID_MAIN = 0,
    BTN_COUNT
} bsp_button_id_t;

bool bsp_button_is_pressed(bsp_button_id_t btn_id);

#ifdef __cplusplus
}
#endif