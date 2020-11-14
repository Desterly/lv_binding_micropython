/*
 * lv_cp_gc_custom_include.h
 * CircuitPython's supervisor module includes a 'decompress' function
 * which by default is included in mpstate.h
 * 
 * That function conflicts with the 'decompress' function found in
 * lv_font/lv_font_fmt_txt preventing compilation.
 * 
 * This wrapper allows compilation to occur without changes to LVGL or CircuitPython
 */
#ifndef CP_GC_H
#define CP_GC_H
#define decompress decompress_lvgl
#include "py/mpstate.h"
#undef decompress
#endif