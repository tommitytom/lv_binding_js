import { getInstance } from "../reconciler";

const eventMap = {};

export const EVENTTYPE_MAP = {
  EVENT_ALL: 0,

  /** Input device events (LVGL 9 values) */
  EVENT_PRESSED: 1 /**< The object has been pressed*/,
  EVENT_PRESSING: 2 /**< The object is being pressed (called continuously while pressing)*/,
  EVENT_PRESS_LOST: 3 /**< The object is still being pressed but slid cursor/finger off of the object */,
  EVENT_SHORT_CLICKED: 4 /**< The object was pressed for a short period of time, then released it. Not called if scrolled.*/,
  EVENT_SINGLE_CLICKED: 5 /**< Sent for first short click within a small distance and short time */,
  EVENT_DOUBLE_CLICKED: 6 /**< Sent for second short click within small distance and short time */,
  EVENT_TRIPLE_CLICKED: 7 /**< Sent for third short click within small distance and short time */,
  EVENT_LONG_PRESSED: 8 /**< Object has been pressed for at least `long_press_time`.  Not called if scrolled.*/,
  EVENT_LONG_PRESSED_REPEAT: 9 /**< Called after `long_press_time` in every `long_press_repeat_time` ms.  Not called if scrolled.*/,
  EVENT_CLICKED: 10 /**< Called on release if not scrolled (regardless to long press)*/,
  EVENT_RELEASED: 11 /**< Called in every cases when the object has been released*/,
  EVENT_SCROLL_BEGIN: 12 /**< Scrolling begins. The event parameter is a pointer to the animation of the scroll. Can be modified*/,
  EVENT_SCROLL_THROW_BEGIN: 13 /**< Scrolling throw begins */,
  EVENT_SCROLL_END: 14 /**< Scrolling ends*/,
  EVENT_SCROLL: 15 /**< Scrolling*/,
  EVENT_GESTURE: 16 /**< A gesture is detected. Get the gesture with `indev_get_gesture_dir(indev_get_act());` */,
  EVENT_KEY: 17 /**< A key is sent to the object. Get the key with `indev_get_key(indev_get_act());`*/,
  EVENT_ROTARY: 18 /**< An encoder or wheel was rotated */,
  EVENT_FOCUSED: 19 /**< The object is focused*/,
  EVENT_DEFOCUSED: 20 /**< The object is defocused*/,
  EVENT_LEAVE: 21 /**< The object is defocused but still selected*/,
  EVENT_HIT_TEST: 22 /**< Perform advanced hit-testing*/,
  EVENT_INDEV_RESET: 23 /**< Indev has been reset */,
  EVENT_HOVER_OVER: 24 /**< Indev hover over object */,
  EVENT_HOVER_LEAVE: 25 /**< Indev hover leave object */,

  /** Drawing events*/
  EVENT_COVER_CHECK: 26 /**< Check if the object fully covers an area. The event parameter is `cover_check_info_t *`.*/,
  EVENT_REFR_EXT_DRAW_SIZE: 27 /**< Get the required extra draw area around the object (e.g. for shadow). The event parameter is `coord_t *` to store the size.*/,
  EVENT_DRAW_MAIN_BEGIN: 28 /**< Starting the main drawing phase*/,
  EVENT_DRAW_MAIN: 29 /**< Perform the main drawing*/,
  EVENT_DRAW_MAIN_END: 30 /**< Finishing the main drawing phase*/,
  EVENT_DRAW_POST_BEGIN: 31 /**< Starting the post draw phase (when all children are drawn)*/,
  EVENT_DRAW_POST: 32 /**< Perform the post draw phase (when all children are drawn)*/,
  EVENT_DRAW_POST_END: 33 /**< Finishing the post draw phase (when all children are drawn)*/,
  EVENT_DRAW_TASK_ADDED: 34 /**< Adding a draw task */,

  /** Special events*/
  EVENT_VALUE_CHANGED: 35 /**< The object's value has changed (i.e. slider moved)*/,
  EVENT_INSERT: 36 /**< A text is inserted to the object. The event data is `char *` being inserted.*/,
  EVENT_REFRESH: 37 /**< Notify the object to refresh something on it (for the user)*/,
  EVENT_READY: 38 /**< A process has finished*/,
  EVENT_CANCEL: 39 /**< A process has been cancelled */,
  EVENT_STATE_CHANGED: 40 /**< The state of the widget changed */,

  /** Other events*/
  EVENT_CREATE: 41 /**< Object is being created */,
  EVENT_DELETE: 42 /**< Object is being deleted*/,
  EVENT_CHILD_CHANGED: 43 /**< Child was removed, added, or its size, position were changed */,
  EVENT_CHILD_CREATED: 44 /**< Child was created, always bubbles up to all parents*/,
  EVENT_CHILD_DELETED: 45 /**< Child was deleted, always bubbles up to all parents*/,
  EVENT_SCREEN_UNLOAD_START: 46 /**< A screen unload started, fired immediately when scr_load is called*/,
  EVENT_SCREEN_LOAD_START: 47 /**< A screen load started, fired when the screen change delay is expired*/,
  EVENT_SCREEN_LOADED: 48 /**< A screen was loaded*/,
  EVENT_SCREEN_UNLOADED: 49 /**< A screen was unloaded*/,
  EVENT_SIZE_CHANGED: 50 /**< Object coordinates/size have changed*/,
  EVENT_STYLE_CHANGED: 51 /**< Object's style has changed*/,
  EVENT_LAYOUT_CHANGED: 52 /**< The children position has changed due to a layout recalculation*/,
  EVENT_GET_SELF_SIZE: 53 /**< Get the internal size of a widget*/,

  _EVENT_LAST: 70 /** Number of default events*/,

  EVENT_PREPROCESS: 0x8000 /** This is a flag that can be set with an event so it's processed
                                      before the class default event processing */,
};

export function registEvent(uid, eventType, fn) {
  eventMap[uid] = eventMap[uid] || {};
  eventMap[uid][eventType] = fn;
}

export function unRegistEvent(uid, eventType) {
  if (!eventType) {
    delete eventMap[uid];
  } else {
    const obj = eventMap[uid];
    obj && delete obj[eventType];
  }
}

export function fireEvent(targetUid, currentTargetUid, eventType, e) {
  const obj = eventMap[currentTargetUid];
  const target = getInstance(targetUid);
  const currentTarget = getInstance(currentTargetUid);
  if (obj) {
    e.target = target;
    e.currentTarget = currentTarget;
    try {
      obj[eventType].call(null, e);
    } catch (err) {
      console.log(err);
    }
  }
}

export function handleEvent(comp, fn, type) {
  if (fn) {
    registEvent(comp.uid, type, fn);
    comp.addEventListener(type);
  } else {
    unRegistEvent(comp.uid, type);
    comp.removeEventListener(type);
  }
}

globalThis.FIRE_QEVENT_CALLBACK = fireEvent;
