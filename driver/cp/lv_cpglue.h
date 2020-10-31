
#ifndef MICROPY_INCLUDED_SHARED_BINDINGS_DISPLAYIO_LVGLGLUE_H
#define MICROPY_INCLUDED_SHARED_BINDINGS_DISPLAYIO_LVGLGLUE_H

#include <stdbool.h>
#include <stdint.h>

#include "py/obj.h"
#include "lib/lv_bindings/lvgl/lvgl.h"
#include "shared-bindings/displayio/Group.h"



typedef struct {
    mp_obj_base_t base;
    displayio_group_t* group;
    uint64_t last_refresh;

} lv_cpglue_t;

extern const mp_obj_type_t lv_cpglue_type;

void lv_cpglue_construct(lv_cpglue_t *self);
void lv_cpglue_tick(void);
// uint32_t common_hal_displayio_lvglglue_get_pixel(displayio_lvglglue_t *bitmap,
//     int16_t x, int16_t y);

// uint16_t common_hal_displayio_lvglglue_get_height(displayio_lvglglue_t *self);

// uint16_t common_hal_displayio_lvglglue_get_width(displayio_lvglglue_t *self);
#endif // MICROPY_INCLUDED_SHARED_BINDINGS_DISPLAYIO_LVGLGLUE_H