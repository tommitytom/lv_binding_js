#pragma once

#include <algorithm>
#include <utility>
#include <vector>

#include "lvgl.h"
#include "private.h"

void NativeRenderUtilInit (JSContext* ctx, JSValue& ns);

#include "native/components/window/window.hpp"