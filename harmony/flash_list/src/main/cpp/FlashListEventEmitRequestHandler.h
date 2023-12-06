#pragma once

#include "RNOH/ArkJS.h"
#include "RNOH/EventEmitRequestHandler.h"
#include "EventEmitters.h"

using namespace facebook;
namespace rnoh {

class FlashListEventEmitRequestHandler : public EventEmitRequestHandler {
  public:
    void handleEvent(EventEmitRequestHandler::Context const &ctx) override {
      if (ctx.eventName != "AutoLayoutView") {
        return;
      }
      ArkJS arkJs(ctx.env);
      auto eventEmitter = ctx.shadowViewRegistry->getEventEmitter<react::AutoLayoutViewEventEmitter>(ctx.tag);
      if (eventEmitter == nullptr) {
        return;
      }
      int offsetStart = (int) arkJs.getDouble(arkJs.getObjectProperty(ctx.payload, "offsetStart"));
      int offsetEnd = (int) arkJs.getDouble(arkJs.getObjectProperty(ctx.payload, "offsetEnd"));
      int blankArea = (int) arkJs.getDouble(arkJs.getObjectProperty(ctx.payload, "blankArea"));
      react::AutoLayoutViewEventEmitter::OnBlankAreaEvent event{offsetStart, offsetEnd, blankArea};
      eventEmitter->onBlankAreaEvent(event);
    };
};
} // namespace rnoh