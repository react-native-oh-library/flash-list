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

#include "CellContainerComponentInstance.h"

namespace rnoh {

    CellContainerComponentInstance::CellContainerComponentInstance(Context context)
        : CppComponentInstance(std::move(context)) {}

    void CellContainerComponentInstance::onChildInserted(ComponentInstance::Shared const &childComponentInstance,
                                                         std::size_t index) {
        CppComponentInstance::onChildInserted(childComponentInstance, index);
        m_stackNode.insertChild(childComponentInstance->getLocalRootArkUINode(), index);
    }

    void CellContainerComponentInstance::onChildRemoved(ComponentInstance::Shared const &childComponentInstance) {
        CppComponentInstance::onChildRemoved(childComponentInstance);
        m_stackNode.removeChild(childComponentInstance->getLocalRootArkUINode());
    };

    FlashListStackNode &CellContainerComponentInstance::getLocalRootArkUINode() { return m_stackNode; }

    void CellContainerComponentInstance::setIndex(int const &index) { this->index = index; }
    int CellContainerComponentInstance::getIndex() { return index; }

    void CellContainerComponentInstance::setLeft(facebook::react::Float const &left) {
        m_layoutMetrics.frame.origin.x = left;
    }
    facebook::react::Float CellContainerComponentInstance::getLeft() { return m_layoutMetrics.frame.origin.x; }
    void CellContainerComponentInstance::setTop(facebook::react::Float const &top) {
        m_layoutMetrics.frame.origin.y = top;
    }
    facebook::react::Float CellContainerComponentInstance::getTop() { return m_layoutMetrics.frame.origin.y; }
    void CellContainerComponentInstance::setRight(facebook::react::Float const &right) {
        m_layoutMetrics.frame.origin.x = right - m_layoutMetrics.frame.size.width;
    }
    facebook::react::Float CellContainerComponentInstance::getRight() {
        return m_layoutMetrics.frame.origin.x + m_layoutMetrics.frame.size.width;
    }
    void CellContainerComponentInstance::setBottom(facebook::react::Float const &bottom) {
        m_layoutMetrics.frame.origin.y = bottom - m_layoutMetrics.frame.size.height;
    }
    facebook::react::Float CellContainerComponentInstance::getBottom() {
        return m_layoutMetrics.frame.origin.y + m_layoutMetrics.frame.size.height;
    }
    void CellContainerComponentInstance::setHeight(facebook::react::Float const &height) {
        m_layoutMetrics.frame.size.height = height;
    }
    facebook::react::Float CellContainerComponentInstance::getHeight() { return m_layoutMetrics.frame.size.height; }
    void CellContainerComponentInstance::setWidth(facebook::react::Float const &width) {
        m_layoutMetrics.frame.size.width = width;
    }
    facebook::react::Float CellContainerComponentInstance::getWidth() { return m_layoutMetrics.frame.size.width; }

    void CellContainerComponentInstance::onPropsChanged(SharedConcreteProps const &props) {
        CppComponentInstance::onPropsChanged(props);
        LOG(INFO) << "[clx] CellContainerComponentInstance::setProps" << props->index;
        this->setIndex(props->index);
    }

} // namespace rnoh