
#include "window.hpp"

static LvBindingJsDisplayData* getDisplayData() {
    lv_display_t* disp = lv_display_get_default();
    return disp ? static_cast<LvBindingJsDisplayData*>(lv_display_get_user_data(disp)) : nullptr;
}

lv_obj_t* GetWindowInstance () {
    LvBindingJsDisplayData* data = getDisplayData();
    return data ? data->windowInstance : nullptr;
};

void WindowInit () {
    lv_display_t* disp_default = lv_display_get_default();

    // Allocate per-display data if not already set
    LvBindingJsDisplayData* data = getDisplayData();
    if (!data) {
        data = new LvBindingJsDisplayData();
        lv_display_set_user_data(disp_default, data);
    }

    lv_obj_t* win = lv_obj_create(lv_scr_act());
    lv_group_add_obj(lv_group_get_default(), win);
    lv_obj_set_style_height(win, lv_display_get_vertical_resolution(disp_default), 0);
    lv_obj_set_style_width(win, lv_display_get_horizontal_resolution(disp_default), 0);
    lv_obj_set_style_pad_all(win, 0, 0);
    lv_obj_set_style_radius(win, 0, 0);
    lv_obj_set_style_border_width(win, 0, 0);

    data->windowInstance = win;
};
