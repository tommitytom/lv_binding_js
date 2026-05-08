#pragma once
#include <stdlib.h>

#include "native/components/component.hpp"
#include "native/core/basic/comp.hpp"

// Per-display data stored via lv_display_set_user_data().
// Allows multi-instance use when each instance has its own LVGL display.
struct LvBindingJsDisplayData {
    lv_obj_t* windowInstance = nullptr;
};

// class Window : public BasicComponent {
//  public:
//   Window(std::string uid);
// };