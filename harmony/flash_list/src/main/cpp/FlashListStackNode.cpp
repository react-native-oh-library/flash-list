#include "FlashListStackNode.h"

#include <glog/logging.h>
#include <memory>

namespace rnoh {

FlashListStackNode::FlashListStackNode()
    : ArkUINode(NativeNodeApi::getInstance()->createNode(
          ArkUI_NodeType::ARKUI_NODE_STACK)),
      m_stackNodeDelegate(nullptr) {
  maybeThrow(NativeNodeApi::getInstance()->registerNodeEvent(
      m_nodeHandle, NODE_ON_CLICK, 0, this));
  maybeThrow(NativeNodeApi::getInstance()->registerNodeEvent(
      m_nodeHandle, NODE_ON_HOVER, 0, this));
}

void FlashListStackNode::insertChild(ArkUINode& child, std::size_t index) {
  maybeThrow(NativeNodeApi::getInstance()->insertChildAt(
      m_nodeHandle, child.getArkUINodeHandle(), static_cast<int32_t>(index)));
}

void FlashListStackNode::addChild(ArkUINode &child){
  maybeThrow(NativeNodeApi::getInstance()->addChild(
      m_nodeHandle, child.getArkUINodeHandle()));
}
void FlashListStackNode::removeChild(ArkUINode& child) {
  maybeThrow(NativeNodeApi::getInstance()->removeChild(
      m_nodeHandle, child.getArkUINodeHandle()));
}

void FlashListStackNode::setStackNodeDelegate(FlashListStackNodeDelegate* stackNodeDelegate) {
  m_stackNodeDelegate = stackNodeDelegate;
}

void FlashListStackNode::onNodeEvent(
    ArkUI_NodeEventType eventType,
    EventArgs& eventArgs) {
  if (eventType == ArkUI_NodeEventType::NODE_ON_CLICK &&
      eventArgs[3].i32 != 2) {
    onClick();
  }
  if (eventType == ArkUI_NodeEventType::NODE_ON_HOVER) {
    if (m_stackNodeDelegate != nullptr) {
      if (eventArgs[0].i32) {
        m_stackNodeDelegate->onHoverIn();
      } else {
        m_stackNodeDelegate->onHoverOut();
      }
    }
  }
}

void FlashListStackNode::onClick() {
  if (m_stackNodeDelegate != nullptr) {
    m_stackNodeDelegate->onClick();
  }
}

FlashListStackNode::~FlashListStackNode() {
  NativeNodeApi::getInstance()->unregisterNodeEvent(
      m_nodeHandle, NODE_ON_CLICK);
  NativeNodeApi::getInstance()->unregisterNodeEvent(
      m_nodeHandle, NODE_ON_HOVER);
}

FlashListStackNode& FlashListStackNode::setAlign(int32_t align) {
  ArkUI_NumberValue value[] = {{.i32 = align}};
  ArkUI_AttributeItem item = {.value = value, .size = 1};
  maybeThrow(NativeNodeApi::getInstance()->setAttribute(
      m_nodeHandle, NODE_STACK_ALIGN_CONTENT, &item));
  return *this;
}
} // namespace rnoh
