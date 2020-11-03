

#include <stdint.h>

#include "py/runtime.h"
#include "py/objproperty.h"

#include "lvdisplayglue.h"
#include "lib/lv_bindings/lvgl/lvgl.h"


STATIC lv_disp_drv_t lv_disp_drv;
STATIC lv_disp_buf_t lv_disp_buf;
STATIC lv_color_t lv_pixel_buf[(LV_HOR_RES_MAX) * 10];
STATIC lv_color_t lv_pixel_buf2[(LV_HOR_RES_MAX) * 10];


STATIC bool _glue_refresh_area(const lv_area_t * lvarea, lv_color_t * color_p,lvdisplayglue_glue_obj_t * user_data) {
    displayio_display_obj_t* display = (displayio_display_obj_t*)user_data->display;
    displayio_area_t clipped = {
            .x1 = lvarea->x1,
            .y1 = lvarea->y1,
            .x2 = lvarea->x2+1,
            .y2 = lvarea->y2+1
    };
    displayio_display_core_set_region_to_update(&display->core, display->set_column_command,
            display->set_row_command, NO_COMMAND, NO_COMMAND, display->data_as_commands, false,
            &clipped, display->SH1107_addressing);
    displayio_display_core_begin_transaction(&display->core);
    if (!display->data_as_commands) {
        display->core.send(display->core.bus, DISPLAY_COMMAND, CHIP_SELECT_TOGGLE_EVERY_BYTE, &display->write_ram_command, 1);
    }
    uint16_t subrectangle_size_bytes;
    if (display->core.colorspace.depth >= 8) {
        subrectangle_size_bytes = displayio_area_size(&clipped) * (display->core.colorspace.depth / 8);
    } else {
        subrectangle_size_bytes = displayio_area_size(&clipped) / (8 / display->core.colorspace.depth);
    }
    display->core.send(display->core.bus, DISPLAY_DATA, CHIP_SELECT_UNTOUCHED, (uint8_t *)color_p, subrectangle_size_bytes);
    displayio_display_core_end_transaction(&display->core);            
    return true;
}

void lvdisplayglue_tick(void) {
    if (lv_disp_drv.user_data != 0) {
        lvdisplayglue_glue_obj_t * user_data = (lvdisplayglue_glue_obj_t*)lv_disp_drv.user_data;
        uint64_t current_time = supervisor_ticks_ms64();
        uint64_t last_refresh = user_data->last_refresh;
        if (last_refresh == 0) last_refresh = current_time;
        uint32_t current_ms_since_real_refresh = current_time - last_refresh;
        user_data->last_refresh = current_time;
        lv_tick_inc(current_ms_since_real_refresh);
        lv_task_handler();
    }
}
void _lv_disp_flush(lv_disp_drv_t * disp, const lv_area_t * area, lv_color_t * color_p) {
    lvdisplayglue_glue_obj_t * user_data = (lvdisplayglue_glue_obj_t*)disp->user_data;
    if (user_data->group->in_group && !user_data->group->hidden) {
        _glue_refresh_area(area, color_p, user_data);
    }
    lv_disp_flush_ready(disp);         /* Indicate you are ready with the flushing*/
}

void lvdisplayglue_construct(lvdisplayglue_glue_obj_t *self) {
    // Create a dummy group to hold
    displayio_display_obj_t* display = &displays[0].display;
    displayio_group_t* lv_group = m_new_obj(displayio_group_t);
    self->group = lv_group;
    self->display = display;
    // Set this to the active group
    common_hal_displayio_display_show(display,lv_group);

    // Create the LVGL Buffer and Display
    lv_init();
    lv_disp_buf_init(&lv_disp_buf, &lv_pixel_buf, &lv_pixel_buf2, LV_HOR_RES_MAX * 10);
    lv_disp_drv_init(&lv_disp_drv);
    lv_disp_drv.flush_cb = _lv_disp_flush;    /*Set your driver function*/
    lv_disp_drv.buffer = &lv_disp_buf;
    lv_disp_drv.hor_res = LV_HOR_RES_MAX;
    lv_disp_drv.ver_res = LV_VER_RES_MAX;
    lv_disp_drv.user_data = (lv_indev_drv_user_data_t)self;
    lv_disp_drv_register(&lv_disp_drv); 
}

void lvdisplayglue_setactive(lvdisplayglue_glue_obj_t *self) {
    displayio_group_t* lv_group = (displayio_group_t*)self->group;
    displayio_display_obj_t* display = (displayio_display_obj_t*)self->display;
    common_hal_displayio_display_show(display,lv_group);
}




