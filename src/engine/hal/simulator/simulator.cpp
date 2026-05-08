#include "lvgl/lvgl.h"
#include "lvgl/src/drivers/sdl/lv_sdl_window.h"
#include "lvgl/src/drivers/sdl/lv_sdl_mouse.h"
#include "lvgl/src/drivers/sdl/lv_sdl_keyboard.h"
#include "lvgl/src/drivers/sdl/lv_sdl_mousewheel.h"

extern "C" {
    LV_IMAGE_DECLARE(mouse_cursor_icon);
}

#define HOR_RES 1024
#define VER_RES 600

void hal_init(void) {
    lv_display_t * disp = lv_sdl_window_create(HOR_RES, VER_RES);

    lv_group_t * g = lv_group_create();
    lv_group_set_default(g);

    lv_theme_t * th = lv_theme_default_init(disp,
                                            lv_palette_main(LV_PALETTE_BLUE),
                                            lv_palette_main(LV_PALETTE_RED),
                                            true,
                                            LV_FONT_DEFAULT);
    lv_display_set_theme(disp, th);

    lv_indev_t * mouse = lv_sdl_mouse_create();

    lv_obj_t * cursor_obj = lv_image_create(lv_screen_active());
    lv_image_set_src(cursor_obj, &mouse_cursor_icon);
    lv_indev_set_cursor(mouse, cursor_obj);

    lv_indev_t * keyboard = lv_sdl_keyboard_create();
    lv_indev_set_group(keyboard, g);

    lv_indev_t * mousewheel = lv_sdl_mousewheel_create();
    lv_indev_set_group(mousewheel, g);
}
