#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "native/components/component.hpp"
#include "native/core/basic/comp.hpp"

// Raw-pixel image widget. JS pushes RGBA / XRGB8888 buffers into setBuffer,
// the underlying lv_image renders them. Use when you need direct
// per-frame pixel control (emulator framebuffer, custom canvases, etc.) —
// for static images use the Image component (PNG/GIF decode).
//
// Buffer ownership: setBuffer always copies the JS bytes into a
// native-owned std::vector. The lv_image_dsc_t points at this vector's
// storage; lv_image_set_src is called once per buffer-shape change so LVGL
// picks up the new descriptor.
class Canvas final : public BasicComponent {
public:
    Canvas(std::string uid, lv_obj_t* parent = nullptr);
    ~Canvas();

    // Copies `bytes` into internal storage and re-points the lv_image_dsc_t.
    // Width/height/stride are inferred from arguments; stride = width*4 (XRGB8888).
    void setBuffer(const uint8_t* data, size_t bytes, uint32_t w, uint32_t h);

    void invalidate();
    void setNearestNeighbor(bool nearest);
    void setInnerAlign(int align);

    void virtual initCompStyle(int32_t type) override;

private:
    lv_image_dsc_t       dsc{};
    std::vector<uint8_t> storage;
    uint32_t             width  = 0;
    uint32_t             height = 0;
    bool                 hasBuffer = false;
};
