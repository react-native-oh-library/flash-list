#pragma once

#include "ShadowNodes.h"
#include <react/renderer/core/ConcreteComponentDescriptor.h>

namespace facebook {
namespace react {

class ComponentDescriptorParameters;
class ComponentDescriptor;

class AutoLayoutViewComponentDescriptor : public ConcreteComponentDescriptor<AutoLayoutViewShadowNode> {
  public:
    AutoLayoutViewComponentDescriptor(ComponentDescriptorParameters const &parameters) 
      : ConcreteComponentDescriptor(parameters) {}
};

class CellContainerComponentDescriptor : public ConcreteComponentDescriptor<CellContainerShadowNode> {
  public:
    CellContainerComponentDescriptor(ComponentDescriptorParameters const &parameters) 
      : ConcreteComponentDescriptor(parameters) {}
};

} // namespace react
} // namespace facebook