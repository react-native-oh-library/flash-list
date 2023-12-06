#pragma once

#include "EventEmitters.h"
#include "Props.h"
#include <react/renderer/components/view/ConcreteViewShadowNode.h>

namespace facebook {
namespace react {

extern const char AutoLayoutViewComponentName[] = "AutoLayoutView";
extern const char CellContainerComponentName[] = "CellContainer";

using AutoLayoutViewShadowNode = ConcreteViewShadowNode<
  AutoLayoutViewComponentName,
  AutoLayoutViewProps,
  AutoLayoutViewEventEmitter>;

using CellContainerShadowNode = ConcreteViewShadowNode<
  CellContainerComponentName,
  CellContainerProps,
  ViewEventEmitter>;

} // namespace react
} // namespace facebook