#include "render_bootstrap.hpp"

#include "native/components/component.hpp"
#include "native/core/animate/animate.hpp"
#include "native/core/dimensions/dimensions.hpp"
#include "native/core/group/group.hpp"
#include "native/core/refresh/refresh.hpp"
#include "native/core/theme/theme.hpp"

#define NATIVE_RENDER_OBJ "NativeRender"

void NativeRenderInit (JSContext* ctx, JSValue ns) {
    JSValue obj = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, ns, NATIVE_RENDER_OBJ, obj);

    NativeComponentInit(ctx, obj);

    NativeEventWrapInit(ctx);

    NativeAnimateInit(ctx, obj);

    NativeGroupInit(ctx, obj);

    NativeDimensionsInit(ctx, obj);

    NativeRenderUtilInit(ctx, obj);

    NativeThemeInit(ctx, obj);

    lv_init(); // idempotent in v9 — safe to call if already initialized
};
