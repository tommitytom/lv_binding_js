#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <mutex>
#include <string>

    #include "lvgl.h"

    #include "private.h"

// NOTE: the old file-scope `static MemoryPool` object pools (animate_pool /
// style_pool) were removed. They were shared per-process, which is illegal in an
// audio-plugin context (one binary, many instances in the same host process), and
// their static destructors crashed at shutdown. The two users now allocate their
// lv_anim_t / lv_style_t objects directly with new/delete.
