import { getComponentByTagName } from "../../components/config";
import { unRegistEvent } from "../event";
import Reconciler from "react-reconciler";
import {
  DefaultEventPriority,
  NoEventPriority,
} from "react-reconciler/constants";

let id = 1;

export const getUid = () => {
  return String(id++);
};

const instanceMap = new Map();

export const getInstance = (uid) => {
  return instanceMap[uid];
};

const noop = () => {};

let currentUpdatePriority = NoEventPriority;

const HostConfig = {
  supportsMutation: true,
  supportsPersistence: false,
  supportsHydration: false,
  isPrimaryRenderer: true,
  supportsMicrotasks: typeof queueMicrotask === "function",
  scheduleMicrotask:
    typeof queueMicrotask === "function" ? queueMicrotask : undefined,
  noTimeout: -1,
  scheduleTimeout: setTimeout,
  cancelTimeout: clearTimeout,

  now: Date.now,
  getPublicInstance: (instance) => instance,
  getRootHostContext: () => ({ name: "rootnode" }),
  getChildHostContext: () => ({}),
  prepareForCommit: () => null,
  resetAfterCommit: noop,
  preparePortalMount: noop,
  shouldSetTextContent: () => false,

  createInstance: (
    type,
    newProps,
    rootContainerInstance,
    _currentHostContext,
    workInProgress,
  ) => {
    const { createInstance } = getComponentByTagName(type);
    const uid = getUid();
    const instance = createInstance(
      newProps,
      rootContainerInstance,
      _currentHostContext,
      workInProgress,
      uid,
    );
    instanceMap[uid] = instance;
    return instance;
  },
  createTextInstance: () => null,

  appendInitialChild: (parent, child) => {
    parent.appendChild(child);
  },
  appendChild: (parent, child) => {
    parent.appendChild(child);
  },
  finalizeInitialChildren: () => true,
  insertBefore: (parent, child, beforeChild) => {
    parent.insertBefore(child, beforeChild);
  },

  appendChildToContainer: (container, child) => {
    container.add(child);
  },
  insertInContainerBefore: (container, child) => {
    container.add(child);
  },
  removeChildFromContainer: (container, child) => {
    container.delete(child);
    if (child.close) {
      child.close();
    }
  },
  clearContainer: (container) => {
    for (const child of container) {
      if (child.close) child.close();
      container.delete(child);
    }
  },

  commitUpdate: (instance, type, oldProps, newProps, internalHandle) => {
    const { commitUpdate } = getComponentByTagName(type);
    return commitUpdate(instance, oldProps, newProps, internalHandle);
  },
  commitTextUpdate: (textInstance, oldText, newText) => {
    textInstance.setText(newText);
  },
  commitMount: (instance, type, newProps, internalInstanceHandle) => {
    const { commitMount } = getComponentByTagName(type);
    return commitMount(instance, newProps, internalInstanceHandle);
  },

  removeChild: (parent, child) => {
    parent?.removeChild(child);
    unRegistEvent(child.uid);
    delete instanceMap[child.uid];
  },
  detachDeletedInstance: noop,

  // Event priority (React 19 replaced getCurrentEventPriority with this trio)
  setCurrentUpdatePriority: (priority) => {
    currentUpdatePriority = priority;
  },
  getCurrentUpdatePriority: () => currentUpdatePriority,
  resolveUpdatePriority: () =>
    currentUpdatePriority !== NoEventPriority
      ? currentUpdatePriority
      : DefaultEventPriority,
  shouldAttemptEagerTransition: () => false,
  trackSchedulerEvent: noop,
  resolveEventType: () => null,
  resolveEventTimeStamp: () => -1.1,
  bindToConsole: (_methodName, args) => args,
  rendererPackageName: "lvgljs",
  rendererVersion: "1.0.0",
  extraDevToolsConfig: null,

  // Scope / form / blur hooks (no-ops for non-DOM renderer)
  getInstanceFromNode: () => null,
  prepareScopeUpdate: noop,
  getInstanceFromScope: () => null,
  beforeActiveInstanceBlur: noop,
  afterActiveInstanceBlur: noop,
  resetFormInstance: noop,
  requestPostPaintCallback: noop,

  // Resource preload / view-transition (React 19) — no-ops
  maySuspendCommit: () => false,
  preloadInstance: () => true,
  startSuspendingCommit: noop,
  suspendInstance: noop,
  waitForCommitToBeReady: () => null,
  NotPendingTransition: null,
  HostTransitionContext: {
    $$typeof: Symbol.for("react.context"),
    Provider: null,
    Consumer: null,
    _currentValue: null,
    _currentValue2: null,
    _threadCount: 0,
  },
};

export default Reconciler(HostConfig);
