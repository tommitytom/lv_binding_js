import reconciler from "../reconciler";
import { ConcurrentRoot } from "react-reconciler/constants";

const containerInfo = new Set();

const onError = (err) => {
  console.error(err);
};

export class Renderer {
  static container;
  static portalContainer;

  static render(element, options) {
    Renderer.container = reconciler.createContainer(
      containerInfo,
      ConcurrentRoot,
      null, // hydrationCallbacks
      false, // isStrictMode
      null, // concurrentUpdatesByDefaultOverride
      "", // identifierPrefix
      onError, // onUncaughtError
      onError, // onCaughtError
      onError, // onRecoverableError
      () => () => {}, // onDefaultTransitionIndicator
      null, // transitionCallbacks
    );

    const parentComponent = null;
    reconciler.updateContainerSync(element, Renderer.container, parentComponent);
  }

  // Tear down the mounted React tree. Unmounts the fiber root (which drives the
  // host-config removeChild path — lv_obj_delete_async + unRegistEvent for every
  // widget), flushes the synchronous work, then drops the container so a later
  // render() starts a fresh root on the (now-empty) shared containerInfo Set.
  // Deferred LVGL deletes still need a pump (lv_timer_handler) to actually free.
  static unmount() {
    if (!Renderer.container) return;
    reconciler.updateContainerSync(null, Renderer.container, null);
    if (reconciler.flushSyncWork) reconciler.flushSyncWork();
    Renderer.container = null;
  }
}
