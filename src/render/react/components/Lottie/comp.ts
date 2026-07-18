import { isValidUrl } from "../../utils/helpers";
import { CommonComponentApi, CommonProps } from "../common/index";
import {
  EVENTTYPE_MAP,
  handleEvent,
  setStyle,
  styleGetterProp,
} from "../config";

const bridge = globalThis[Symbol.for('lvgljs')];
const NativeLottie = bridge.NativeRender.NativeComponents.Lottie;

async function fetchJson(url) {
  const resp = await fetch(url, {
    headers: {
      "Content-Type": "application/json",
    },
  });
  return await resp.arrayBuffer();
}

export type LottieProps = CommonProps & {
  /** Path or URL to a Lottie animation JSON file. */
  src: string;
  /**
   * ThorVG rasterization resolution (the internal render buffer). Distinct from
   * the widget's laid-out size (set via `style`) — LVGL scales this buffer to the
   * widget box. Keep it modest; ThorVG re-rasterizes it every advanced frame.
   */
  width?: number;
  height?: number;
  /** Loop forever (default) vs play through once. */
  loop?: boolean;
}

function setLottieProps(comp, newProps: LottieProps, oldProps: LottieProps) {
  const setter = {
    ...CommonComponentApi({ compName: "Lottie", comp, newProps, oldProps }),
    onClick(fn) {
      handleEvent(comp, fn, EVENTTYPE_MAP.EVENT_CLICKED);
    },
    width(w) {
      if (typeof w === "number" && (w !== oldProps.width || newProps.height !== oldProps.height)) {
        comp.setRenderSize(w, newProps.height ?? w);
      }
    },
    height(h) {
      if (typeof h === "number" && (h !== oldProps.height || newProps.width !== oldProps.width)) {
        comp.setRenderSize(newProps.width ?? h, h);
      }
    },
    loop(val) {
      if (val !== oldProps.loop) comp.setLoop(val !== false);
    },
    src(url) {
      if (url && url !== oldProps.src) {
        if (!isValidUrl(url)) {
          if (!path.isAbsolute(url)) {
            url = path.resolve(__dirname, url);
          }
          fs.readFile(url, { encoding: "binary" })
            .then((data) => {
              comp.setSrcData(data.buffer);
            })
            .catch((e) => {
              console.log("setLottie error", e);
            });
        } else {
          fetchJson(url)
            .then((buffer) => comp.setSrcData(Buffer.from(buffer).buffer))
            .catch(console.warn);
        }
      }
    },
  };
  // Apply render size before the source so the first parse lands at the right
  // resolution.
  const order = ["width", "height", "loop", "src"];
  order.forEach((key) => {
    if (newProps.hasOwnProperty(key) && setter[key]) {
      setter[key](newProps[key]);
    }
  });
  Object.keys(setter).forEach((key) => {
    if (!order.includes(key) && newProps.hasOwnProperty(key)) {
      setter[key](newProps[key]);
    }
  });
  comp.dataset = {};
  Object.keys(newProps).forEach((prop) => {
    const index = prop.indexOf("data-");
    if (index === 0) {
      comp.dataset[prop.substring(5)] = newProps[prop];
    }
  });
}

export class LottieComp extends NativeLottie {
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
  setProps(newProps: LottieProps, oldProps: LottieProps) {
    setLottieProps(this, newProps, oldProps);
  }
  insertBefore(child, beforeChild) {}
  static tagName = "Lottie";
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
      compName: "Lottie",
      styleType: type,
      oldStyleSheet: null,
      isInit: false,
    });
  }
  moveToFront() {
    super.moveToFront();
  }
  moveToBackground() {
    super.moveToBackground();
  }
  scrollIntoView() {
    super.scrollIntoView();
  }
}
