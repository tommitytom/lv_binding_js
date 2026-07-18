
#include "lottie.hpp"

Lottie::Lottie(std::string uid, lv_obj_t* parent): BasicComponent(uid) {
    this->type = COMP_TYPE_LOTTIE;
    this->uid  = uid;
    this->instance = lv_lottie_create(parent != nullptr ? parent : GetWindowInstance());
    lv_group_add_obj(lv_group_get_default(), this->instance);

    lv_obj_remove_flag(this->instance, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    lv_obj_add_flag(this->instance, (lv_obj_flag_t)(LV_OBJ_FLAG_EVENT_BUBBLE | LV_OBJ_FLAG_CLICKABLE));
    lv_obj_set_user_data(this->instance, this);

    // A default render buffer so a src can be applied before JS sets a size.
    setRenderSize(128, 128);
    this->initStyle(LV_PART_MAIN);
};

void Lottie::setRenderSize(uint32_t w, uint32_t h) {
    if (w == 0 || h == 0) return;
    if (drawBuf_ != nullptr && w == renderW_ && h == renderH_) return;

    lv_draw_buf_t* prev = drawBuf_;
    drawBuf_ = lv_draw_buf_create(w, h, LV_COLOR_FORMAT_ARGB8888, LV_STRIDE_AUTO);
    renderW_ = w;
    renderH_ = h;

    // Re-point the widget at the new buffer BEFORE freeing the old one, so the
    // lottie never briefly references a destroyed draw buffer.
    lv_lottie_set_draw_buf(this->instance, drawBuf_);
    if (prev != nullptr) {
        lv_draw_buf_destroy(prev);
    }
    // A buffer swap resets the render target; re-parse so ThorVG re-inits at the
    // new resolution.
    if (!srcJson_.empty()) {
        reapplySrc();
    }
};

void Lottie::setSrcData(const uint8_t* json, size_t len) {
    if (json == nullptr || len == 0) return;
    srcJson_.assign(json, json + len);
    srcJson_.push_back('\0');  // lv_lottie expects a nul-terminated source array
    if (drawBuf_ == nullptr) {
        setRenderSize(renderW_ != 0 ? renderW_ : 128, renderH_ != 0 ? renderH_ : 128);
    }
    reapplySrc();
};

void Lottie::reapplySrc() {
    if (srcJson_.empty()) return;
    // srcJson_ carries a trailing NUL we appended; pass the logical JSON length.
    lv_lottie_set_src_data(this->instance, srcJson_.data(), srcJson_.size() - 1);
    setLoop(loop_);
};

void Lottie::setLoop(bool loop) {
    loop_ = loop;
    // The animation only exists once a source has been parsed.
    lv_anim_t* anim = lv_lottie_get_anim(this->instance);
    if (anim != nullptr) {
        lv_anim_set_repeat_count(anim, loop ? LV_ANIM_REPEAT_INFINITE : 0);
    }
};

Lottie::~Lottie() {
    // The lv_obj itself is deleted async on React unmount (see BasicComponent),
    // not here — this frees only the draw buffer we own.
    if (drawBuf_ != nullptr) {
        lv_draw_buf_destroy(drawBuf_);
        drawBuf_ = nullptr;
    }
};

void Lottie::initCompStyle(int32_t type) {
};
