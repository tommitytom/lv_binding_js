#pragma once
#include <cstdint>
#include <vector>

#include "native/components/component.hpp"
#include "native/core/basic/comp.hpp"

// Lottie: a ThorVG-rendered vector animation widget (lv_lottie). Requires
// LV_USE_LOTTIE in the active lv_conf.h (which pulls in lv_canvas + ThorVG).
// The widget owns an ARGB8888 draw buffer that ThorVG rasterizes each frame
// into; the software draw unit then blits that buffer like any image. Frames
// advance off the shared lv_timer_handler pump, so there is no extra thread.
class Lottie final : public BasicComponent {
 public:
  Lottie(std::string uid, lv_obj_t* parent = nullptr);
  ~Lottie();

  // Parse + (re)start a Lottie animation from an in-memory JSON buffer. The
  // bytes are copied into native storage and retained for the widget's lifetime
  // (a later setRenderSize re-applies them at the new resolution).
  void setSrcData(const uint8_t* json, size_t len);

  // (Re)allocate the ARGB8888 buffer ThorVG rasterizes into. This is the
  // animation's internal render resolution, independent of the widget's
  // laid-out size — LVGL scales the buffer to the widget box.
  void setRenderSize(uint32_t w, uint32_t h);

  // Loop forever (default) vs play through once.
  void setLoop(bool loop);

  void virtual initCompStyle(int32_t type);

 private:
  void reapplySrc();  // re-parse the retained JSON (after a buffer swap)

  lv_draw_buf_t* drawBuf_ = nullptr;
  std::vector<uint8_t> srcJson_;  // retained bytes + trailing NUL
  uint32_t renderW_ = 0;
  uint32_t renderH_ = 0;
  bool loop_ = true;
};
