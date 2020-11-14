#include "LVGlue_display.h"

// Glue Type
const mp_obj_type_t lvglue_display_type;

STATIC void mp_lvglue_display_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    (void)kind;
    //lvglue_display_obj_t *self = MP_OBJ_TO_PTR(self_in);
    mp_print_str(print, "Glue()");
}

STATIC mp_obj_t mp_lvglue_display_make_new(const mp_obj_type_t *type, size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    lvglue_display_obj_t *self = m_new_obj(lvglue_display_obj_t);
    self->base.type = &lvglue_display_type;
    self->last_refresh = 0;
    lvglue_display_construct(self);
    return MP_OBJ_FROM_PTR(self);
}

// Glue Set Active
STATIC mp_obj_t mp_lvglue_display_setactive(mp_obj_t self_in) {
    lvglue_display_obj_t *self = MP_OBJ_TO_PTR(self_in);
    lvglue_display_setactive(self);
    return mp_const_none;
}

MP_DEFINE_CONST_FUN_OBJ_1(lvglue_display_setactive_obj, mp_lvglue_display_setactive);

STATIC const mp_rom_map_elem_t lvglue_display_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_setActive), MP_ROM_PTR(&lvglue_display_setactive_obj) },
};

STATIC MP_DEFINE_CONST_DICT(lvglue_display_locals_dict, lvglue_display_locals_dict_table);

const mp_obj_type_t lvglue_display_type = {
    { &mp_type_type },
    .name = MP_QSTR_display,
    .print = mp_lvglue_display_print,
    .make_new = mp_lvglue_display_make_new,
    .locals_dict = (mp_obj_dict_t*)&lvglue_display_locals_dict,
};

// Module lvglue

STATIC const mp_rom_map_elem_t lvglue_globals_table[] = {
    { MP_OBJ_NEW_QSTR(MP_QSTR___name__), MP_OBJ_NEW_QSTR(MP_QSTR_lvglue) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_display), (mp_obj_t)&lvglue_display_type },

};

STATIC MP_DEFINE_CONST_DICT(
    mp_module_lvglue_globals,
    lvglue_globals_table);

const mp_obj_module_t lvglue_module = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&mp_module_lvglue_globals
};

MP_REGISTER_MODULE(MP_QSTR_lvglue, lvglue_module, MICROPY_PY_LVGL);

