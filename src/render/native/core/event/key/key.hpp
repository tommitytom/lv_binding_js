#pragma once

#include "lvgl.h"
#include "private.h"

#include <string>

void NativeKeyEventWrapInit(JSContext* ctx);
JSValue WrapKeyEvent(lv_event_t* e, std::string uid);
