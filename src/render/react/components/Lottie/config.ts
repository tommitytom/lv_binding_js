import { LvgljsComponentConfig } from "../config";
import { LottieComp, LottieProps } from "./comp";

export default class LottieConfig implements LvgljsComponentConfig<LottieProps, LottieComp> {
  tagName = "Lottie";
  native = null;
  shouldSetTextContent() {
    return false;
  }
  createInstance(newProps: LottieProps, rootInstance, context, workInProgress, uid) {
    const instance = new LottieComp({ uid });
    instance.setProps(newProps, {});
    return instance;
  }
  commitMount(instance, newProps: LottieProps, internalInstanceHandle) {}
  commitUpdate(instance, oldProps: LottieProps, newProps: LottieProps, finishedWork) {
    instance.setProps(newProps, oldProps);
  }
  setProps(newProps: LottieProps, oldProps: LottieProps) {}
  insertBefore(child, beforeChild) {}
  appendInitialChild(child) {}
  appendChild(child) {}
  removeChild(child) {}
}
