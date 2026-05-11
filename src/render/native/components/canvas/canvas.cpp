#include "canvas.hpp"

#include <cstring>

Canvas::Canvas(std::string uid, lv_obj_t* parent) : BasicComponent(uid) {
    this->type = COMP_TYPE_IMAGE; // closest existing tag; Canvas piggybacks on lv_image
    this->uid = uid;
    this->instance = lv_image_create(parent != nullptr ? parent : GetWindowInstance());
    lv_group_add_obj(lv_group_get_default(), this->instance);

    lv_obj_add_flag(this->instance, (lv_obj_flag_t)(LV_OBJ_FLAG_EVENT_BUBBLE | LV_OBJ_FLAG_CLICKABLE));
    lv_obj_remove_flag(this->instance, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    lv_obj_set_user_data(this->instance, this);

    // Sensible defaults for emulator framebuffer use: nearest-neighbor + CONTAIN.
    // JS can override via setNearestNeighbor / setInnerAlign.
    lv_image_set_antialias(this->instance, false);
    lv_image_set_inner_align(this->instance, LV_IMAGE_ALIGN_CONTAIN);
    lv_image_set_pivot(this->instance, 0, 0);

    this->initStyle(LV_PART_MAIN);
}

Canvas::~Canvas() {
    // No raw heap to free; std::vector cleans itself up.
}

void Canvas::setBuffer(const uint8_t* data, size_t bytes, uint32_t w, uint32_t h) {
    if (data == nullptr || bytes == 0) return;
    const size_t expected = static_cast<size_t>(w) * h * 4;
    if (bytes < expected) return; // buffer too small for declared dims

    if (storage.size() != expected) storage.assign(expected, 0u);
    std::memcpy(storage.data(), data, expected);

    const bool dimsChanged = (w != width) || (h != height) || !hasBuffer;
    width  = w;
    height = h;

    std::memset(&dsc, 0, sizeof(dsc));
    dsc.header.cf     = LV_COLOR_FORMAT_NATIVE; // XRGB8888 with LV_COLOR_DEPTH=32
    dsc.header.w      = w;
    dsc.header.h      = h;
    dsc.header.stride = w * 4;
    dsc.data_size     = static_cast<uint32_t>(expected);
    dsc.data          = storage.data();

    lv_image_cache_drop(&dsc);
    // Set the src on every frame, not just when dims change. LVGL caches
    // decoded image data keyed by the src pointer; even with cache_drop,
    // some scaled/aligned image paths (notably LV_IMAGE_ALIGN_CONTAIN with
    // integer scaling) retain a stale internal blit when the descriptor
    // doesn't get re-applied. Re-applying set_src forces a full refresh
    // and fixes intermittent stale-region artefacts seen with raw RGBA
    // emulator framebuffers (e.g. LSDJ chain↔song page transitions).
    lv_image_set_src(this->instance, &dsc);
    hasBuffer = true;
    lv_obj_invalidate(this->instance);
    (void)dimsChanged;
}

void Canvas::invalidate() {
    lv_obj_invalidate(this->instance);
}

void Canvas::setNearestNeighbor(bool nearest) {
    lv_image_set_antialias(this->instance, !nearest);
}

void Canvas::setInnerAlign(int align) {
    lv_image_set_inner_align(this->instance, static_cast<lv_image_align_t>(align));
}

void Canvas::initCompStyle(int32_t type) {
    // No component-specific defaults beyond what the constructor already set.
}
