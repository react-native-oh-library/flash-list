#include "EventEmitters.h"

namespace facebook {
namespace react {

void AutoLayoutViewEventEmitter::onBlankAreaEvent(OnBlankAreaEvent event) const {
  dispatchEvent("blankAreaEvent", [event=std::move(event)](jsi::Runtime &runtime) {
    auto payload = jsi::Object(runtime);
    payload.setProperty(runtime, "offsetStart", event.offsetStart);
    payload.setProperty(runtime, "offsetEnd", event.offsetEnd);
    payload.setProperty(runtime, "blankArea", event.blankArea);
    return payload;
  });
}

} // namespace react
} // namespace facebook