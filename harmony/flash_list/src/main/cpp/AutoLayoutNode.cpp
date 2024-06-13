/**
 * MIT License
 *
 * Copyright (C) 2024 Huawei Device Co., Ltd.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "AutoLayoutNode.h"
#include <memory>
#include <glog/logging.h>

namespace rnoh {

AutoLayoutNode::AutoLayoutNode()
    : ArkUINode(NativeNodeApi::getInstance()->createNode(ArkUI_NodeType::ARKUI_NODE_STACK)) {
    maybeThrow(NativeNodeApi::getInstance()->registerNodeEvent(m_nodeHandle, NODE_EVENT_ON_APPEAR, 0, this));
}

void AutoLayoutNode::insertChild(ArkUINode &child, std::size_t index) {
    maybeThrow(NativeNodeApi::getInstance()->insertChildAt(m_nodeHandle, child.getArkUINodeHandle(), index));
}

void AutoLayoutNode::removeChild(ArkUINode &child) {
    maybeThrow(NativeNodeApi::getInstance()->removeChild(m_nodeHandle, child.getArkUINodeHandle()));
}

void AutoLayoutNode::onNodeEvent(ArkUI_NodeEventType eventType, EventArgs &eventArgs) {
    if (eventType == ArkUI_NodeEventType::NODE_EVENT_ON_APPEAR) {
        m_autoLayoutNodeDelegate->onAppear();
    }
}

void AutoLayoutNode::setAutoLayoutNodeDelegate(AutoLayoutNodeDelegate *scrollNodeDelegate) {
    m_autoLayoutNodeDelegate = scrollNodeDelegate;
}

AutoLayoutNode::~AutoLayoutNode() {
    NativeNodeApi::getInstance()->unregisterNodeEvent(m_nodeHandle, NODE_EVENT_ON_APPEAR);
}

void AutoLayoutNode::setAlign(int32_t align) {
    ArkUI_NumberValue value[] = {{.i32 = align}};
    ArkUI_AttributeItem item = {.value = value, .size = 1};
    maybeThrow(NativeNodeApi::getInstance()->setAttribute(m_nodeHandle, NODE_STACK_ALIGN_CONTENT, &item));
}
} // namespace rnoh
