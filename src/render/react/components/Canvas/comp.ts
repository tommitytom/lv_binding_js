import { CommonProps, OnClickEvent } from "../common";
import {
  EVENTTYPE_MAP,
  handleEvent,
  setStyle,
  styleGetterProp,
} from "../config";

const bridge = globalThis[Symbol.for('lvgljs')];
const NativeCanvas = bridge.NativeRender.NativeComponents.Canvas;

export type CanvasProps = CommonProps & {
  onClick?: (event: OnClickEvent) => void;
  align?: { type: number; pos: [number, number] };
  alignTo?: { type: number; pos: [number, number]; parent: any };
  /** When true (default), nearest-neighbor scaling — crisp pixels. */
  nearestNeighbor?: boolean;
  /**
   * lv_image_inner_align value. Defaults to LV_IMAGE_ALIGN_CONTAIN (preserve
   * aspect ratio, fit within widget). Other useful values:
   *   LV_IMAGE_ALIGN_STRETCH = 13, LV_IMAGE_ALIGN_CONTAIN = 14, LV_IMAGE_ALIGN_COVER = 15.
   */
  innerAlign?: number;
};

function setCanvasProps(comp, newProps: CanvasProps, oldProps: CanvasProps) {
  const setter = {
    set style(styleSheet) {
      setStyle({
        comp,
        styleSheet,
        compName: "Canvas",
        styleType: 0x0000,
        oldStyleSheet: oldProps.style,
      });
    },
    set onClick(fn) {
      handleEvent(comp, fn, EVENTTYPE_MAP.EVENT_CLICKED);
    },
    set align({ type, pos = [0, 0] }) {
      if (
        !type ||
        (type === oldProps.align?.type &&
          pos[0] === oldProps.align?.pos?.[0] &&
          pos[1] === oldProps.align?.pos?.[1])
      )
        return;
      comp.align(type, pos);
    },
    set alignTo({ type, pos = [0, 0], parent }) {
      if (
        !type ||
        (type === oldProps.alignTo?.type &&
          pos[0] === (oldProps.alignTo?.pos?.[0] || 0) &&
          pos[1] === (oldProps.alignTo?.pos?.[1] || 0) &&
          parent?.uid === oldProps.alignTo?.parent?.uid)
      )
        return;
      comp.alignTo(type, pos, parent);
    },
    set nearestNeighbor(val: boolean) {
      if (val === oldProps.nearestNeighbor) return;
      comp.setNearestNeighbor(val !== false);
    },
    set innerAlign(val: number) {
      if (val === oldProps.innerAlign) return;
      if (typeof val === "number") comp.setInnerAlign(val);
    },
  };
  Object.assign(setter, newProps);
  comp.dataset = {};
  Object.keys(newProps).forEach((prop) => {
    const index = prop.indexOf("data-");
    if (index === 0) {
      comp.dataset[prop.substring(5)] = newProps[prop];
    }
  });
}

export class CanvasComp extends NativeCanvas {
  constructor({ uid }) {
    super({ uid });
    this.uid = uid;

    const style = super.style;
    const that = this;
    this.style = new Proxy(this, {
      get(obj, prop) {
        if (styleGetterProp.includes(prop)) {
          return style[prop].call(that);
        }
      },
    });
  }

  setProps(newProps: CanvasProps, oldProps: CanvasProps) {
    setCanvasProps(this, newProps, oldProps);
  }

  /**
   * Push a new pixel buffer into the canvas. Bytes are copied into native
   * storage; the JS ArrayBuffer is not retained.
   *
   * @param buffer  XRGB8888 bytes (LV_COLOR_FORMAT_NATIVE on LV_COLOR_DEPTH=32),
   *                length must be `width * height * 4` (or larger; trailing
   *                bytes are ignored).
   */
  setBuffer(buffer: ArrayBuffer | ArrayBufferView, width: number, height: number) {
    const ab = ArrayBuffer.isView(buffer)
      ? buffer.buffer.slice(buffer.byteOffset, buffer.byteOffset + buffer.byteLength)
      : buffer;
    super.setBuffer(ab, width, height);
  }

  invalidate() {
    super.invalidate();
  }

  static tagName = "Canvas";

  // Canvas is a leaf widget — children are conceptually drawn into the buffer
  // by the JS owner, not as LVGL siblings. Stub the child methods.
  insertBefore(child, beforeChild) {}
  appendInitialChild(child) {}
  appendChild(child) {}
  removeChild(child) {}

  close() {
    super.close();
  }

  setStyle(style, type = 0x0000) {
    setStyle({
      comp: this,
      styleSheet: style,
      compName: "Canvas",
      styleType: type,
      oldStyleSheet: null,
      isInit: false,
    });
  }

  moveToFront() { super.moveToFront(); }
  moveToBackground() { super.moveToBackground(); }
  scrollIntoView() { super.scrollIntoView(); }
}
