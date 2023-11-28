#pragma once

#include "RNOHCorePackage/ComponentBinders/ViewComponentJNapiBinder.h"
#include "Props.h"

namespace rnoh {

class FlashListViewNapiBinder : public ViewComponentNapiBinder {
  public:
    napi_value createProps(napi_env env, facebook::react::ShadowView const shadowView) override {
      napi_value napiViewProps = ViewComponentNapiBinder::createProps(env, shadowView);
      if (auto props = std::dynamic_pointer_cast<const facebook::react::AutoLayoutViewProps>(shadowView.props)) {
        return ArkJS(env)
          .getObjectBuilder(napiViewProps)
          .addProperty("horizontal", props->horizontal)
          .addProperty("scrollOffset", props->scrollOffset)
          .addProperty("windowSize", props->windowSize)
          .addProperty("renderAheadOffset", props->renderAheadOffset)
          .addProperty("enableInstrumentation", props->enableInstrumentation)
          .addProperty("disableAutoLayout", props->disableAutoLayout)
          .build();
      } else if (auto props = std::dynamic_pointer_cast<const facebook::react::CellContainerProps>(shadowView.props)) {
        return ArkJS(env)
          .getObjectBuilder(napiViewProps)
          .addProperty("index", props->index)
          .build();
      }
        return napiViewProps;
    };
};

} // namespace rnoh