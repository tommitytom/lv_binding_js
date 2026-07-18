#include "lottie.hpp"

static JSClassID LottieClassID;

WRAPPED_JS_SETSTYLE(Lottie, "Lottie")
WRAPPED_JS_AddEventListener(Lottie, "Lottie")
WRAPPED_JS_Align(Lottie, "Lottie")
WRAPPED_JS_Align_To(Lottie, "Lottie")
STYLE_INFO(Lottie, "Lottie")
WRAPPED_MOVE_TO_FRONT(Lottie, "Lottie")
WRAPPED_MOVE_TO_BACKGROUND(Lottie, "Lottie")
WRAPPED_SCROLL_INTO_VIEW(Lottie, "Lottie")
WRAPPED_JS_CLOSE_COMPONENT(Lottie, "Lottie")

// lottie.setSrcData(arrayBuffer) — the raw Lottie JSON bytes. Copied into
// native storage; the JS ArrayBuffer is not retained.
static JSValue NativeCompSetSrcData(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv) {
    if (argc < 1 || !JS_IsObject(argv[0])) return JS_UNDEFINED;
    size_t size = 0;
    uint8_t* buf = JS_GetArrayBuffer(ctx, &size, argv[0]);
    if (!buf) return JS_UNDEFINED;

    COMP_REF* ref = (COMP_REF*)JS_GetOpaque(this_val, LottieClassID);
    ((Lottie*)(ref->comp))->setSrcData(buf, size);
    return JS_NewBool(ctx, 1);
}

// lottie.setRenderSize(width, height) — the ThorVG rasterization resolution.
static JSValue NativeCompSetRenderSize(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv) {
    if (argc < 2 || !JS_IsNumber(argv[0]) || !JS_IsNumber(argv[1])) return JS_UNDEFINED;
    int32_t w = 0, h = 0;
    JS_ToInt32(ctx, &w, argv[0]);
    JS_ToInt32(ctx, &h, argv[1]);
    if (w <= 0 || h <= 0) return JS_UNDEFINED;

    COMP_REF* ref = (COMP_REF*)JS_GetOpaque(this_val, LottieClassID);
    ((Lottie*)(ref->comp))->setRenderSize(static_cast<uint32_t>(w), static_cast<uint32_t>(h));
    return JS_UNDEFINED;
}

// lottie.setLoop(bool)
static JSValue NativeCompSetLoop(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv) {
    if (argc < 1) return JS_UNDEFINED;
    bool loop = JS_ToBool(ctx, argv[0]) != 0;
    COMP_REF* ref = (COMP_REF*)JS_GetOpaque(this_val, LottieClassID);
    ((Lottie*)(ref->comp))->setLoop(loop);
    return JS_UNDEFINED;
}

static const JSCFunctionListEntry ComponentProtoFuncs[] = {
    TJS_CFUNC_DEF("nativeSetStyle", 0, NativeCompSetStyle),
    TJS_CFUNC_DEF("addEventListener", 0, NativeCompAddEventListener),
    TJS_CFUNC_DEF("setSrcData", 0, NativeCompSetSrcData),
    TJS_CFUNC_DEF("setRenderSize", 0, NativeCompSetRenderSize),
    TJS_CFUNC_DEF("setLoop", 0, NativeCompSetLoop),
    TJS_CFUNC_DEF("align", 0, NativeCompSetAlign),
    TJS_CFUNC_DEF("alignTo", 0, NativeCompSetAlignTo),
    JS_OBJECT_DEF("style", style_funcs, countof(style_funcs), JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE),
    TJS_CFUNC_DEF("getBoundingClientRect", 0, GetStyleBoundClinetRect),
    TJS_CFUNC_DEF("moveToFront", 0, NativeCompMoveToFront),
    TJS_CFUNC_DEF("moveToBackground", 0, NativeCompMoveToBackground),
    TJS_CFUNC_DEF("scrollIntoView", 0, NativeCompScrollIntoView),
    TJS_CFUNC_DEF("close", 0, NativeCompCloseComponent),
};

static const JSCFunctionListEntry ComponentClassFuncs[1] = {{}};
static JSValue LottieConstructor(JSContext* ctx, JSValueConst new_target, int argc, JSValueConst* argv) {
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
        proto = JS_GetClassProto(ctx, LottieClassID);
    } else {
        proto = JS_GetPropertyStr(ctx, new_target, "prototype");
        if (JS_IsException(proto)) goto fail;
    }

    obj = JS_NewObjectProtoClass(ctx, proto, LottieClassID);
    JS_FreeValue(ctx, proto);
    if (JS_IsException(obj)) goto fail;

    s = (COMP_REF*)js_mallocz(ctx, sizeof(*s));
    s->uid = uid;
    s->comp = new Lottie(uid, NULL);
    JS_FreeCString(ctx, uid);
    if (!s) goto fail;

    JS_SetOpaque(obj, s);
    LV_LOG_USER("Lottie %s created", uid);
    return obj;

fail:
    JS_FreeValue(ctx, obj);
    return JS_EXCEPTION;
}

static void LottieFinalizer(JSRuntime* rt, JSValue val) {
    COMP_REF* th = (COMP_REF*)JS_GetOpaque(val, LottieClassID);
    LV_LOG_USER("Lottie %s release", th->uid);
    if (th) {
        delete static_cast<Lottie*>(th->comp);
        js_free_rt(rt, th);
    }
}

static JSClassDef LottieClass = {
    .class_name = "Lottie",
    .finalizer = LottieFinalizer,
};

void NativeComponentLottieInit(JSContext* ctx, JSValue ns) {
    JS_NewClassID(JS_GetRuntime(ctx), &LottieClassID);
    JS_NewClass(JS_GetRuntime(ctx), LottieClassID, &LottieClass);
    JSValue proto = JS_NewObject(ctx);
    JS_SetPropertyFunctionList(ctx, proto, ComponentProtoFuncs, countof(ComponentProtoFuncs));
    JS_SetClassProto(ctx, LottieClassID, proto);

    JSValue obj = JS_NewCFunction2(ctx, LottieConstructor, "Lottie", 1, JS_CFUNC_constructor, 0);
    JS_SetConstructor(ctx, obj, proto);
    JS_SetPropertyFunctionList(ctx, obj, ComponentClassFuncs, 0);
    JS_DefinePropertyValueStr(ctx, ns, "Lottie", obj, JS_PROP_C_W_E);
}
