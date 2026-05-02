# lv_binding_js (LVGL v9 fork)

Write [LVGL](https://github.com/lvgl/lvgl) UIs with JavaScript and React.

This is a fork of [lvgl/lv_binding_js](https://github.com/lvgl/lv_binding_js) with the native C++ component layer ported to **LVGL v9**. It uses React's virtual DOM concept to manipulate LVGL UI components, providing a familiar web development experience.

## What's changed in this fork

- **LVGL v9 API port** — All native C++ components updated from LVGL v8 to v9
- **txiki.js v26.4.0** — Updated JS runtime (QuickJS + libuv + libwebsockets + WAMR)
- **Multi-instance safe** — Window instance stored per-display via `lv_display_set_user_data()`
- **Embeddable** — Native component bridge can be compiled as a static library and integrated into host applications (no standalone executable required)

### LVGL v9 API changes applied

- `lv_obj_clear_flag` → `lv_obj_remove_flag`
- `lv_obj_del_async` → `lv_obj_delete_async`
- `lv_btn_create` → `lv_obj_create` (button widget removed in v9)
- `lv_img_*` → `lv_image_*` (create, set_src, set_scale, set_rotation, set_pivot)
- `lv_disp_t` → `lv_display_t`, `lv_disp_get_default` → `lv_display_get_default`
- Display driver: `->driver->hor_res` → `lv_display_get_horizontal_resolution()`
- Event access: `e->target` → `lv_event_get_target(e)`
- Image decoder: `lv_img_decoder_*` → `lv_image_decoder_*`
- Style: `lv_style_set_transform_zoom` → `lv_style_set_transform_scale`
- QuickJS-ng: `JS_IsArray(ctx, val)` → `JS_IsArray(val)`, `JS_BOOL` → `bool`

### Known v9 limitations

- **Chart axis ticks**: `lv_chart_set_axis_tick()` removed — v9 uses `lv_scale` widgets instead (not yet implemented)
- **Chart draw events**: `LV_EVENT_DRAW_PART_BEGIN` restructured — custom tick labels not implemented
- **GIF**: `lv_gif_create()` was a third-party extension — falls back to `lv_image_create()` (no animation)
- **Theme colors**: `lv_theme_get_color_primary()` removed — uses hardcoded default blue

## Features

- All LVGL built-in components accessible from JavaScript
- Full support for LVGL flex and grid layouts
- CSS-like styling (write styles like HTML/CSS)
- Dynamic image loading
- Full LVGL animation support
- React hooks (useState, useRef, useCallback, etc.)

## Example

```tsx
import { View, Text, Slider, Button, Render } from "lvgljs-ui";
import React, { useState } from "react";

function App() {
    const [value, setValue] = useState(0);

    return (
        <View style={{
            width: "100%", height: "100%",
            "background-color": "#1a1a2e",
            display: "flex", "flex-direction": "column",
            "align-items": "center", padding: 20,
        }}>
            <Text style={{ "text-color": "#ffffff", "font-size": 24 }}>
                {`Value: ${value}`}
            </Text>
            <Slider
                style={{ width: 300, "margin-top": 20 }}
                range={[0, 100]}
                onChange={(e) => setValue(e.value)}
            />
            <Button
                style={{ "margin-top": 20, "background-color": "#4fc3f7" }}
                onClick={() => setValue(0)}
            >
                <Text>Reset</Text>
            </Button>
        </View>
    );
}

Render.render(<App />);
```

## Building

### As an embedded library

The native component bridge can be compiled as a static library for integration into host applications. See [lvgl-template-plugin](https://github.com/tommitytom/lvgl-template-plugin) for an example of embedding in a DPF audio plugin.

### Standalone (simulator)

The standalone simulator build (`lvgljs` executable) has not been ported to v9 yet — the HAL layer still references the deprecated `lv_drivers` library. The native component bridge and React layer are fully functional when embedded.

### JS bundle

Install dependencies and bundle TSX/JSX files with esbuild:

```bash
pnpm install   # or npm install
node build.js  # bundles demo apps
```

## Components

View, Text, Button, Image, Slider, Arc, Switch, Textarea, Keyboard, Checkbox, Dropdownlist, ProgressBar, Roller, Line, Calendar, Chart, Tabs, GIF, Mask

See the [doc/component/](./doc/component/) directory for per-component documentation.

## Style

CSS-like properties for layout, color, typography, borders, shadows, transforms, and animations. Supports flex and grid layouts.

See the [doc/style/](./doc/style/) directory for style documentation.

## Dependencies

- [LVGL v9.5.0](https://github.com/lvgl/lvgl) — UI toolkit
- [txiki.js v26.4.0](https://github.com/nicovank/txiki.js) — JavaScript runtime
  - [QuickJS-ng](https://github.com/nicovank/quickjs) — JavaScript engine
  - [libuv](https://github.com/libuv/libuv) — Platform abstraction layer
  - [libwebsockets](https://github.com/nicovank/libwebsockets) — HTTP/WebSocket client
  - [mbedtls](https://github.com/Mbed-TLS/mbedtls) — TLS/crypto
  - [WAMR](https://github.com/nicovank/wasm-micro-runtime) — WebAssembly runtime
