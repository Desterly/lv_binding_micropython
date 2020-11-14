
#ifndef LVGLUE_DISPLAY_H
#define LVGLUE_DISPLAY_H

//#include "lvglue.h"
#include "shared-module/displayio/__init__.h"
#include "shared-bindings/displayio/Group.h"



typedef struct _lvglue_display_obj_t {
    mp_obj_base_t base;
    displayio_group_t* group;
    displayio_display_obj_t* display;
    uint64_t last_refresh;
} lvglue_display_obj_t;

extern uint64_t supervisor_ticks_ms64(void);

void lvglue_display_construct(lvglue_display_obj_t *self);
void lvglue_display_tick(void);
void lvglue_display_setactive(lvglue_display_obj_t *self); 


#endif // LVGLUE_DISPLAY_H