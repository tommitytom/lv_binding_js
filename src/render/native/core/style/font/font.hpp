#pragma once

#include <vector>

#include "lvgl.h"
#include "private.h"

/* Available fonts from lv_conf.h: 12, 14, 16, 18, 22, 24, 32.
 * Unavailable sizes are mapped to the nearest available font. */
static const lv_font_t* builtin_font_list[] = {
    &lv_font_montserrat_12,  /* 8  -> 12 (nearest available) */
    &lv_font_montserrat_12,  /* 10 -> 12 (nearest available) */
    &lv_font_montserrat_12,  /* 12 (available) */
    &lv_font_montserrat_14,  /* 14 (available) */
    &lv_font_montserrat_16,  /* 16 (available) */
    &lv_font_montserrat_18,  /* 18 (available) */
    &lv_font_montserrat_18,  /* 20 -> 18 (nearest available) */
    &lv_font_montserrat_22,  /* 22 (available) */
    &lv_font_montserrat_24,  /* 24 (available) */
    &lv_font_montserrat_24,  /* 26 -> 24 (nearest available) */
    &lv_font_montserrat_24,  /* 28 -> 24 (nearest available) */
    &lv_font_montserrat_32,  /* 30 -> 32 (nearest available) */
    &lv_font_montserrat_32,  /* 32 (available) */
    &lv_font_montserrat_32,  /* 34 -> 32 (nearest available) */
    &lv_font_montserrat_32,  /* 36 -> 32 (nearest available) */
    &lv_font_montserrat_32,  /* 38 -> 32 (nearest available) */
    &lv_font_montserrat_32,  /* 40 -> 32 (nearest available) */
    &lv_font_montserrat_32,  /* 42 -> 32 (nearest available) */
    &lv_font_montserrat_32,  /* 44 -> 32 (nearest available) */
    &lv_font_montserrat_32,  /* 46 -> 32 (nearest available) */
    &lv_font_montserrat_32,  /* 48 -> 32 (nearest available) */
};

void CompSetFontSize (lv_obj_t* comp, lv_style_t* style, JSContext* ctx, JSValue obj);

void CompSetFontSize1 (lv_obj_t* comp, lv_style_t* style, JSContext* ctx, JSValue obj);