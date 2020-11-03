#include "LVDisplayGlue.h"

// Glue Type
const mp_obj_type_t lvdisplayglue_glue_type;

STATIC void lvdisplayglue_glue_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    (void)kind;
    //lvdisplayglue_glue_obj_t *self = MP_OBJ_TO_PTR(self_in);
    mp_print_str(print, "Glue()");
}

STATIC mp_obj_t lvdisplayglue_glue_make_new(const mp_obj_type_t *type, size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    lvdisplayglue_glue_obj_t *self = m_new_obj(lvdisplayglue_glue_obj_t);
    self->base.type = &lvdisplayglue_glue_type;
    self->last_refresh = 0;
    lvdisplayglue_construct(self);
    return MP_OBJ_FROM_PTR(self);
}

// Glue Set Active
STATIC mp_obj_t lvdisplayglue_glue_setactive(mp_obj_t self_in) {
    lvdisplayglue_glue_obj_t *self = MP_OBJ_TO_PTR(self_in);
    lvdisplayglue_setactive(self);
    return mp_const_none;
}

MP_DEFINE_CONST_FUN_OBJ_1(lvdisplayglue_glue_setactive_obj, lvdisplayglue_glue_setactive);

STATIC const mp_rom_map_elem_t lvdisplayglue_glue_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_setActive), MP_ROM_PTR(&lvdisplayglue_glue_setactive_obj) },
};

STATIC MP_DEFINE_CONST_DICT(lvdisplayglue_glue_locals_dict, lvdisplayglue_glue_locals_dict_table);

const mp_obj_type_t lvdisplayglue_glue_type = {
    { &mp_type_type },
    .name = MP_QSTR_Glue,
    .print = lvdisplayglue_glue_print,
    .make_new = lvdisplayglue_glue_make_new,
    .locals_dict = (mp_obj_dict_t*)&lvdisplayglue_glue_locals_dict,
};

// Module lvdisplayglue
STATIC const mp_rom_map_elem_t lvdisplayglue_globals_table[] = {
    { MP_OBJ_NEW_QSTR(MP_QSTR___name__), MP_OBJ_NEW_QSTR(MP_QSTR_lvdisplayglue) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_Glue), (mp_obj_t)&lvdisplayglue_glue_type },

};
        
STATIC MP_DEFINE_CONST_DICT(
    mp_module_lvdisplayglue_globals,
    lvdisplayglue_globals_table);

const mp_obj_module_t lvdisplayglue_module = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&mp_module_lvdisplayglue_globals
};

MP_REGISTER_MODULE(MP_QSTR_lvdisplayglue, lvdisplayglue_module, MICROPY_PY_LVGL);

