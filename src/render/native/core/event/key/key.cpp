#include "key.hpp"

#include "engine.hpp"
#include "native/core/event/event.hpp"

WRAPPED_STOPPROPAGATION

static JSClassID WrapKeyEventID;

static void EventFinalizer(JSRuntime* rt, JSValue val) {
}

static JSClassDef KeyEventWrapClass = {
    .class_name = "Key",
    .finalizer = EventFinalizer,
};

static JSValue GetKey(JSContext* ctx, JSValueConst this_val) {
    lv_event_t* e = static_cast<lv_event_t*>(JS_GetOpaque(this_val, WrapKeyEventID));
    if (!e) return JS_NewUint32(ctx, 0);
    uint32_t key = lv_event_get_key(e);
    return JS_NewUint32(ctx, key);
}

JSValue WrapKeyEvent(lv_event_t* e, std::string uid) {
    TJSRuntime* qrt = GetRuntime();
    JSContext* ctx = qrt->ctx;
    JSValue proto = JS_GetClassProto(ctx, WrapKeyEventID);
    JSValue obj = JS_NewObjectProtoClass(ctx, proto, WrapKeyEventID);
    JS_FreeValue(ctx, proto);
    JS_SetOpaque(obj, e);
    return obj;
}

static const JSCFunctionListEntry component_proto_funcs[] = {
    TJS_CGETSET_DEF("key", GetKey, NULL),
    TJS_CFUNC_DEF("stopPropagation", 0, NativeEventStopPropagation),
};

void NativeKeyEventWrapInit(JSContext* ctx) {
    JS_NewClassID(JS_GetRuntime(ctx), &WrapKeyEventID);
    JS_NewClass(JS_GetRuntime(ctx), WrapKeyEventID, &KeyEventWrapClass);
    JSValue proto = JS_NewObject(ctx);
    JS_SetPropertyFunctionList(ctx, proto, component_proto_funcs, countof(component_proto_funcs));
    JS_SetClassProto(ctx, WrapKeyEventID, proto);
}
