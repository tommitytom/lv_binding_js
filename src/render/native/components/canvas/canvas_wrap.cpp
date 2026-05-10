#include "canvas.hpp"

static JSClassID CanvasClassID;

WRAPPED_JS_SETSTYLE(Canvas, "Canvas")
WRAPPED_JS_AddEventListener(Canvas, "Canvas")
WRAPPED_JS_Align(Canvas, "Canvas")
WRAPPED_JS_Align_To(Canvas, "Canvas")
STYLE_INFO(Canvas, "Canvas")
WRAPPED_MOVE_TO_FRONT(Canvas, "Canvas")
WRAPPED_MOVE_TO_BACKGROUND(Canvas, "Canvas")
WRAPPED_SCROLL_INTO_VIEW(Canvas, "Canvas")
WRAPPED_JS_CLOSE_COMPONENT(Canvas, "Canvas")

// canvas.setBuffer(arrayBuffer, width, height) — copies the JS bytes
// into native-owned storage and re-points the lv_image_dsc_t. JS retains
// its ArrayBuffer; native does not pin it.
static JSValue NativeCompSetBuffer(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv) {
    if (argc < 3 || !JS_IsObject(argv[0]) || !JS_IsNumber(argv[1]) || !JS_IsNumber(argv[2]))
        return JS_UNDEFINED;

    size_t size = 0;
    uint8_t* buf = JS_GetArrayBuffer(ctx, &size, argv[0]);
    if (!buf) return JS_UNDEFINED;

    int32_t w = 0, h = 0;
    JS_ToInt32(ctx, &w, argv[1]);
    JS_ToInt32(ctx, &h, argv[2]);
    if (w <= 0 || h <= 0) return JS_UNDEFINED;

    COMP_REF* ref = (COMP_REF*)JS_GetOpaque(this_val, CanvasClassID);
    ((Canvas*)(ref->comp))->setBuffer(buf, size, static_cast<uint32_t>(w), static_cast<uint32_t>(h));
    return JS_NewBool(ctx, 1);
}

static JSValue NativeCompSetNearestNeighbor(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv) {
    if (argc < 1) return JS_UNDEFINED;
    COMP_REF* ref = (COMP_REF*)JS_GetOpaque(this_val, CanvasClassID);
    bool nn = JS_ToBool(ctx, argv[0]) != 0;
    ((Canvas*)(ref->comp))->setNearestNeighbor(nn);
    return JS_UNDEFINED;
}

static JSValue NativeCompSetInnerAlign(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv) {
    if (argc < 1 || !JS_IsNumber(argv[0])) return JS_UNDEFINED;
    int32_t align = 0;
    JS_ToInt32(ctx, &align, argv[0]);
    COMP_REF* ref = (COMP_REF*)JS_GetOpaque(this_val, CanvasClassID);
    ((Canvas*)(ref->comp))->setInnerAlign(align);
    return JS_UNDEFINED;
}

static JSValue NativeCompCanvasInvalidate(JSContext* ctx, JSValueConst this_val, int, JSValueConst*) {
    COMP_REF* ref = (COMP_REF*)JS_GetOpaque(this_val, CanvasClassID);
    ((Canvas*)(ref->comp))->invalidate();
    return JS_UNDEFINED;
}

static const JSCFunctionListEntry ComponentProtoFuncs[] = {
    TJS_CFUNC_DEF("nativeSetStyle", 0, NativeCompSetStyle),
    TJS_CFUNC_DEF("addEventListener", 0, NativeCompAddEventListener),
    TJS_CFUNC_DEF("setBuffer", 0, NativeCompSetBuffer),
    TJS_CFUNC_DEF("setNearestNeighbor", 0, NativeCompSetNearestNeighbor),
    TJS_CFUNC_DEF("setInnerAlign", 0, NativeCompSetInnerAlign),
    TJS_CFUNC_DEF("invalidate", 0, NativeCompCanvasInvalidate),
    TJS_CFUNC_DEF("align", 0, NativeCompSetAlign),
    TJS_CFUNC_DEF("alignTo", 0, NativeCompSetAlignTo),
    JS_OBJECT_DEF("style", style_funcs, countof(style_funcs), JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE),
    TJS_CFUNC_DEF("getBoundingClientRect", 0, GetStyleBoundClinetRect),
    TJS_CFUNC_DEF("moveToFront", 0, NativeCompMoveToFront),
    TJS_CFUNC_DEF("moveToBackground", 0, NativeCompMoveToBackground),
    TJS_CFUNC_DEF("scrollIntoView", 0, NativeCompScrollIntoView),
    TJS_CFUNC_DEF("close", 0, NativeCompCloseComponent),
};

static const JSCFunctionListEntry ComponentClassFuncs[] = {};

static JSValue CanvasConstructor(JSContext* ctx, JSValueConst new_target, int argc, JSValueConst* argv) {
    JSValue proto;
    JSValue obj;
    JSValue arg;
    JSValue jsUid;
    const char* uid = "";

    COMP_REF* s;

    if (JS_IsObject(argv[0])) {
        arg = argv[0];
        jsUid = JS_GetPropertyStr(ctx, arg, "uid");
        if (JS_IsString(jsUid)) {
            uid = JS_ToCString(ctx, jsUid);
            JS_FreeValue(ctx, jsUid);
        }
    }

    if (JS_IsUndefined(new_target)) {
        proto = JS_GetClassProto(ctx, CanvasClassID);
    } else {
        proto = JS_GetPropertyStr(ctx, new_target, "prototype");
        if (JS_IsException(proto)) goto fail;
    }

    obj = JS_NewObjectProtoClass(ctx, proto, CanvasClassID);
    JS_FreeValue(ctx, proto);
    if (JS_IsException(obj)) goto fail;

    s = (COMP_REF*)js_mallocz(ctx, sizeof(*s));
    s->uid = uid;
    s->comp = new Canvas(uid, NULL);
    JS_FreeCString(ctx, uid);
    if (!s) goto fail;

    JS_SetOpaque(obj, s);
    LV_LOG_USER("Canvas %s created", uid);
    return obj;

fail:
    JS_FreeValue(ctx, obj);
    return JS_EXCEPTION;
}

static void CanvasFinalizer(JSRuntime* rt, JSValue val) {
    COMP_REF* th = (COMP_REF*)JS_GetOpaque(val, CanvasClassID);
    LV_LOG_USER("Canvas %s release", th->uid);
    if (th) {
        delete static_cast<Canvas*>(th->comp);
        js_free_rt(rt, th);
    }
}

static JSClassDef CanvasClass = {
    .class_name = "Canvas",
    .finalizer = CanvasFinalizer,
};

void NativeComponentCanvasInit(JSContext* ctx, JSValue ns) {
    JS_NewClassID(JS_GetRuntime(ctx), &CanvasClassID);
    JS_NewClass(JS_GetRuntime(ctx), CanvasClassID, &CanvasClass);
    JSValue proto = JS_NewObject(ctx);
    JS_SetPropertyFunctionList(ctx, proto, ComponentProtoFuncs, countof(ComponentProtoFuncs));
    JS_SetClassProto(ctx, CanvasClassID, proto);

    JSValue obj = JS_NewCFunction2(ctx, CanvasConstructor, "Canvas", 1, JS_CFUNC_constructor, 0);
    JS_SetConstructor(ctx, obj, proto);
    JS_SetPropertyFunctionList(ctx, obj, ComponentClassFuncs, countof(ComponentClassFuncs));
    JS_DefinePropertyValueStr(ctx, ns, "Canvas", obj, JS_PROP_C_W_E);
}
