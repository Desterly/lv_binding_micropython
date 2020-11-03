
#ifndef LVDisplayGlue_H
#define LVDisplayGlue_H

#include "py/obj.h"
#include "shared-module/displayio/__init__.h"
#include "shared-bindings/displayio/Group.h"



typedef struct _lvdisplayglue_glue_obj_t {
    mp_obj_base_t base;
    displayio_group_t* group;
    displayio_display_obj_t* display;
    uint64_t last_refresh;
} lvdisplayglue_glue_obj_t;

extern uint64_t supervisor_ticks_ms64(void);

void lvdisplayglue_construct(lvdisplayglue_glue_obj_t *self);
void lvdisplayglue_tick(void);
void lvdisplayglue_setactive(lvdisplayglue_glue_obj_t *self); 


#endif // LVDisplayGlue_H