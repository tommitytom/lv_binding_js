#pragma once

#include "render/native/core/style/style.hpp"
#include "render/native/core/utils/utils.hpp"

extern JSClassID GroupClassID;

void NativeGroupInit(JSContext* ctx, JSValue ns);

JSValue NativeSetKeyboardGroup(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv);
