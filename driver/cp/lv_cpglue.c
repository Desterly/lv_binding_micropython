

#include <stdint.h>

#include "py/runtime.h"
#include "py/objproperty.h"
//#include "supervisor/shared/translate.h"
#include "shared-module/displayio/__init__.h"
#include "lv_cpglue.h"
#include "shared-bindings/displayio/ColorConverter.h"
#include "supervisor/shared/tick.h"

STATIC lv_disp_drv_t lv_disp_drv;
STATIC lv_disp_buf_t lv_disp_buf;
// Buffers used for LVGL Processing
STATIC lv_color_t lv_pixel_buf[(LV_HOR_RES_MAX) * 10];
STATIC lv_color_t lv_pixel_buf2[(LV_HOR_RES_MAX) * 10];


STATIC bool _glue_refresh_area(const lv_area_t * lvarea, lv_color_t * color_p) {
    displayio_display_obj_t* display = &displays[0].display;
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

void lv_cpglue_tick(void) {
    if (lv_disp_drv.user_data != 0) {
        lv_cpglue_t * user_data = (lv_cpglue_t*)lv_disp_drv.user_data;
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
    lv_cpglue_t * user_data = (lv_cpglue_t*)disp->user_data;
    if (user_data->group->in_group && !user_data->group->hidden) {
        _glue_refresh_area(area, color_p);
    }
    lv_disp_flush_ready(disp);         /* Indicate you are ready with the flushing*/
}

void lv_cpglue_construct(lv_cpglue_t *self) {
    // Create a dummy group to hold
    displayio_group_t *lv_group = m_new_obj(displayio_group_t);
    self->base.type = &displayio_group_type;

    self->group = lv_group;
    // Set this to the active group
    displayio_display_obj_t* display = &displays[0].display;
    //common_hal_displayio_display_set_auto_refresh(display, false);
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



STATIC mp_obj_t lv_cpglue_make_new(const mp_obj_type_t *type, size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    mp_arg_check_num(n_args, kw_args, 0, 0, false);
    lv_cpglue_t *self = m_new_obj(lv_cpglue_t);
    self->base.type = &lv_cpglue_type;
    self->last_refresh = 0;
    lv_cpglue_construct(self);

    return MP_OBJ_FROM_PTR(self);
}

STATIC const mp_rom_map_elem_t lv_cpglue_locals_dict_table[] = {
//    { MP_ROM_QSTR(MP_QSTR_height), MP_ROM_PTR(&displayio_lvglglue_height_obj) },
//    { MP_ROM_QSTR(MP_QSTR_width), MP_ROM_PTR(&displayio_lvglglue_width_obj) },
};
STATIC MP_DEFINE_CONST_DICT(lv_cpglue_locals_dict, lv_cpglue_locals_dict_table);

const mp_obj_type_t lv_cpglue_type = {
    { &mp_type_type },
    .name = MP_QSTR_CPGlue,
    .make_new = lv_cpglue_make_new,
    .locals_dict = (mp_obj_dict_t*)&lv_cpglue_locals_dict,
};
