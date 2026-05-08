#include "group.hpp"

#include "native/components/component.hpp"
#include "native/core/basic/comp.hpp"

JSClassID GroupClassID;

static JSValue NativeGroupAdd(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv) {
    if (argc < 1 || !JS_IsObject(argv[0])) return JS_UNDEFINED;
    lv_group_t* group = (lv_group_t*)JS_GetOpaque(this_val, GroupClassID);
    if (!group) return JS_UNDEFINED;
    JSClassID class_id;
    COMP_REF* ref = (COMP_REF*)JS_GetAnyOpaque(argv[0], &class_id);
    if (!ref || !ref->comp) return JS_UNDEFINED;
    lv_obj_t* obj = static_cast<BasicComponent*>(ref->comp)->instance;
    if (obj) lv_group_add_obj(group, obj);
    return JS_UNDEFINED;
}

static JSValue NativeGroupRemove(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv) {
    if (argc < 1 || !JS_IsObject(argv[0])) return JS_UNDEFINED;
    JSClassID class_id;
    COMP_REF* ref = (COMP_REF*)JS_GetAnyOpaque(argv[0], &class_id);
    if (!ref || !ref->comp) return JS_UNDEFINED;
    lv_obj_t* obj = static_cast<BasicComponent*>(ref->comp)->instance;
    if (obj) lv_group_remove_obj(obj);
    return JS_UNDEFINED;
}

static JSValue NativeGroupFocusObj(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv) {
    if (argc < 1 || !JS_IsObject(argv[0])) return JS_UNDEFINED;
    JSClassID class_id;
    COMP_REF* ref = (COMP_REF*)JS_GetAnyOpaque(argv[0], &class_id);
    if (!ref || !ref->comp) return JS_UNDEFINED;
    lv_obj_t* obj = static_cast<BasicComponent*>(ref->comp)->instance;
    if (obj) lv_group_focus_obj(obj);
    return JS_UNDEFINED;
}

static JSValue NativeGroupDestroy(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv) {
    lv_group_t* group = (lv_group_t*)JS_GetOpaque(this_val, GroupClassID);
    if (group) {
        lv_group_delete(group);
        JS_SetOpaque(this_val, nullptr);
    }
    return JS_UNDEFINED;
}

static const JSCFunctionListEntry GroupProtoFuncs[] = {
    TJS_CFUNC_DEF("add", 1, NativeGroupAdd),
    TJS_CFUNC_DEF("remove", 1, NativeGroupRemove),
    TJS_CFUNC_DEF("focus", 1, NativeGroupFocusObj),
    TJS_CFUNC_DEF("destroy", 0, NativeGroupDestroy),
};

static JSValue GroupConstructor(JSContext* ctx, JSValueConst new_target, int argc, JSValueConst* argv) {
    JSValue proto;
    if (JS_IsUndefined(new_target)) {
        proto = JS_GetClassProto(ctx, GroupClassID);
    } else {
        proto = JS_GetPropertyStr(ctx, new_target, "prototype");
        if (JS_IsException(proto)) return JS_EXCEPTION;
    }
    JSValue obj = JS_NewObjectProtoClass(ctx, proto, GroupClassID);
    JS_FreeValue(ctx, proto);
    if (JS_IsException(obj)) return JS_EXCEPTION;
    lv_group_t* group = lv_group_create();
    JS_SetOpaque(obj, group);
    return obj;
}

static void GroupFinalizer(JSRuntime* rt, JSValue val) {
    lv_group_t* group = (lv_group_t*)JS_GetOpaque(val, GroupClassID);
    if (group) lv_group_delete(group);
}

static JSClassDef GroupClass = {
    .class_name = "Group",
    .finalizer = GroupFinalizer,
};

void NativeGroupInit(JSContext* ctx, JSValue ns) {
    JS_NewClassID(JS_GetRuntime(ctx), &GroupClassID);
    JS_NewClass(JS_GetRuntime(ctx), GroupClassID, &GroupClass);
    JSValue proto = JS_NewObject(ctx);
    JS_SetPropertyFunctionList(ctx, proto, GroupProtoFuncs, countof(GroupProtoFuncs));
    JS_SetClassProto(ctx, GroupClassID, proto);

    JSValue obj = JS_NewCFunction2(ctx, GroupConstructor, "Group", 0, JS_CFUNC_constructor, 0);
    JS_SetConstructor(ctx, obj, proto);
    JS_DefinePropertyValueStr(ctx, ns, "Group", obj, JS_PROP_C_W_E);
}

JSValue NativeSetKeyboardGroup(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv) {
    lv_group_t* target = nullptr;
    if (argc >= 1 && JS_IsObject(argv[0])) {
        target = (lv_group_t*)JS_GetOpaque(argv[0], GroupClassID);
    }
    if (!target) {
        target = lv_group_get_default();
    }
    lv_indev_t* indev = nullptr;
    while ((indev = lv_indev_get_next(indev)) != nullptr) {
        if (lv_indev_get_type(indev) == LV_INDEV_TYPE_KEYPAD) {
            lv_indev_set_group(indev, target);
        }
    }
    return JS_UNDEFINED;
}
