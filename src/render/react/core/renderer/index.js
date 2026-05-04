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
}
