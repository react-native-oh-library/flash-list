#pragma once

#include <react/renderer/components/view/ViewEventEmitter.h>
#include <jsi/jsi.h>

namespace facebook {
namespace react {

class JSI_EXPORT AutoLayoutViewEventEmitter : public ViewEventEmitter {
  public:
    using ViewEventEmitter::ViewEventEmitter;
    
    struct OnBlankAreaEvent {
      int offsetStart;
      int offsetEnd;
      int blankArea;
    };
    
    void onBlankAreaEvent(OnBlankAreaEvent value) const;
};

} // namespace react
} // namespace facebook