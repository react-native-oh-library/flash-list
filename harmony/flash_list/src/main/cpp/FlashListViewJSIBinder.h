#pragma once
#include "RNOHCorePackage/ComponentBinders/ViewComponentJSIBinder.h"

namespace rnoh {
class FlashListViewJSIBinder : public ViewComponentJSIBinder {
  facebook::jsi::Object createNativeProps(facebook::jsi::Runtime &rt) override {
    auto object = ViewComponentJSIBinder::createNativeProps(rt);
    object.setProperty(rt, "horizontal", "boolean");
    object.setProperty(rt, "scrollOffset", "float");
    object.setProperty(rt, "windowSize", "float");
    object.setProperty(rt, "enableInstrumentation", "boolean");
    object.setProperty(rt, "disableAutoLayout", "boolean");
    object.setProperty(rt, "renderAheadOffset", "float");
    object.setProperty(rt, "index", "float");
    return object;
  }

  facebook::jsi::Object createDirectEventTypes(facebook::jsi::Runtime &rt) override {
    facebook::jsi::Object events(rt);
    events.setProperty(rt, "topBlankAreaEvent", createDirectEvent(rt, "onBlankAreaEvent"));
    return events;
  }
};
} // namespace rnoh  