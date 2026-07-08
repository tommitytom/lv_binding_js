import { HostConfig } from "react-reconciler";

export { handleEvent } from "../core/event";
export { unRegistEvent, EVENTTYPE_MAP } from "../core/event";
export { setStyle } from "../core/style";
import * as React from 'react';

const components = new Map<LvgljsComponentConfig<any, any>['tagName'], LvgljsComponentConfig<any, any>>();

export const getComponentByTagName = (tagName) => {
  const config = components.get(tagName);
  if (!config) {
    throw `Unknown component ${tagName}`;
  }
  return config;
};

export function registerComponent<Props, Comp>(
  config: LvgljsComponentConfig<Props, Comp>): React.ComponentType<Props> | string {
  if (components.has(config.tagName)) {
    throw `A component with tagName: ${config.tagName} already exists. This base component will be ignored`;
  }
  components.set(config.tagName, config);
  return config.tagName;
}

export function registerComponents<Props, Comp>(configs: LvgljsComponentConfig<Props, Comp>[]) {
  configs.forEach((config) => {
    if (components.has(config.tagName)) {
      throw `A component with tagName: ${config.tagName} already exists. This base component will be ignored`;
    }
    components.set(config.tagName, config);
  });
}

export const EAlignType = {
  ALIGN_DEFAULT: 0,
  ALIGN_TOP_LEFT: 1,
  ALIGN_TOP_MID: 2,
  ALIGN_TOP_RIGHT: 3,
  ALIGN_BOTTOM_LEFT: 4,
  ALIGN_BOTTOM_MID: 5,
  ALIGN_BOTTOM_RIGHT: 6,
  ALIGN_LEFT_MID: 7,
  ALIGN_RIGHT_MID: 8,
  ALIGN_CENTER: 9,

  ALIGN_OUT_TOP_LEFT: 10,
  ALIGN_OUT_TOP_MID: 11,
  ALIGN_OUT_TOP_RIGHT: 12,
  ALIGN_OUT_BOTTOM_LEFT: 13,
  ALIGN_OUT_BOTTOM_MID: 14,
  ALIGN_OUT_BOTTOM_RIGHT: 15,
  ALIGN_OUT_LEFT_TOP: 16,
  ALIGN_OUT_LEFT_MID: 17,
  ALIGN_OUT_LEFT_BOTTOM: 18,
  ALIGN_OUT_RIGHT_TOP: 19,
  ALIGN_OUT_RIGHT_MID: 20,
  ALIGN_OUT_RIGHT_BOTTOM: 21,
};

export const STYLE_TYPE = {
  PART_MAIN: 0x000000,
  PART_SCROLLBAR: 0x010000,
  PART_INDICATOR: 0x020000,
  PART_KNOB: 0x030000,
  PART_SELECTED: 0x040000,
  PART_ITEMS: 0x050000,
  PART_TICKS: 0x060000,
  PART_CURSOR: 0x070000,

  // Must match LVGL's lv_state_t enum (deps/lvgl/src/core/lv_obj_style.h) — these are used as
  // lv_obj_add_style() state selectors, so a stale value binds a style to the wrong state (e.g. an
  // onHoveredStyle that never triggers). LVGL 9.x shifted these up from the old v8 values.
  STATE_DEFAULT: 0x0000,
  STATE_CHECKED: 0x0004, // 1 << 2
  STATE_FOCUSED: 0x0008, // 1 << 3
  STATE_FOCUS_KEY: 0x0010, // 1 << 4
  STATE_EDITED: 0x0020, // 1 << 5
  STATE_HOVERED: 0x0040, // 1 << 6
  STATE_PRESSED: 0x0080, // 1 << 7
  STATE_SCROLLED: 0x0100, // 1 << 8
  STATE_DISABLED: 0x0200, // 1 << 9
};

export const EDropdownlistDirection = {
  none: 0x00,
  left: 1 << 0,
  right: 1 << 1,
  top: 1 << 2,
  bottom: 1 << 3,
  horizontal: (1 << 0) | (1 << 1),
  vertical: (1 << 2) | (1 << 3),
  all: (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3),
};

export const EDropdownListArrowDirection = {
  up: 0,
  right: 1,
  down: 2,
  left: 3,
};

export const ELvKey = {
  LV_KEY_UP: 17,
  LV_KEY_DOWN: 18,
  LV_KEY_RIGHT: 19,
  LV_KEY_LEFT: 20,
  LV_KEY_ESC: 27,
  LV_KEY_DEL: 127,
  LV_KEY_BACKSPACE: 8,
  LV_KEY_ENTER: 10,
  LV_KEY_NEXT: 9,
  LV_KEY_PREV: 11,
  LV_KEY_HOME: 2,
  LV_KEY_END: 3,
} as const;

export type LvKey = typeof ELvKey[keyof typeof ELvKey];

export const styleGetterProp = ["height", "width", "left", "top"];

export type LvgljsComponentConfig<ComponentProps, ComponentInstance> = Pick<
  HostConfig<any, ComponentProps, any, ComponentInstance, any, any, any, any, any, any, any, any>,
  | "shouldSetTextContent"
  | "createInstance"
  | "commitMount"
  | "insertBefore"
  | "appendInitialChild"
  | "appendChild"
  | "removeChild"
> & {
  tagName: string;
  commitUpdate(
    instance: ComponentInstance,
    oldProps: ComponentProps,
    newProps: ComponentProps,
    internalHandle: any,
  ): void;
};
