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

#include "AutoLayoutViewComponentInstance.h"
#include "RNOHCorePackage/ComponentInstances/ViewComponentInstance.h"
#include "folly/Synchronized.h"
#include "folly/synchronization/Lock.h"
#include <mutex>
#include <sys/param.h>

namespace rnoh {

    AutoLayoutViewComponentInstance::AutoLayoutViewComponentInstance(Context context)
        : CppComponentInstance(std::move(context)) {
        m_autoLayoutNode.setAutoLayoutNodeDelegate(this);
    }

    void AutoLayoutViewComponentInstance::onChildInserted(ComponentInstance::Shared const &childComponentInstance,
                                                          std::size_t index) {
        CppComponentInstance::onChildInserted(childComponentInstance, index);
        m_autoLayoutNode.insertChild(childComponentInstance->getLocalRootArkUINode(), index);
    }

    AutoLayoutNode &AutoLayoutViewComponentInstance::getLocalRootArkUINode() { return m_autoLayoutNode; }

    void AutoLayoutViewComponentInstance::onChildRemoved(ComponentInstance::Shared const &childComponentInstance) {
        CppComponentInstance::onChildRemoved(childComponentInstance);
        m_autoLayoutNode.removeChild(childComponentInstance->getLocalRootArkUINode());
    };

    void AutoLayoutViewComponentInstance::finalizeUpdates() {
        if (parentScrollView != nullptr) {
            onAppear();
        }
        ComponentInstance::finalizeUpdates();
    }

    facebook::react::Float AutoLayoutViewComponentInstance::getLeft() { return m_layoutMetrics.frame.origin.x; }
    facebook::react::Float AutoLayoutViewComponentInstance::getTop() { return m_layoutMetrics.frame.origin.y; }
    facebook::react::Float AutoLayoutViewComponentInstance::getRight() {
        return m_layoutMetrics.frame.origin.x + m_layoutMetrics.frame.size.width;
    }
    facebook::react::Float AutoLayoutViewComponentInstance::getBottom() {
        return m_layoutMetrics.frame.origin.y + m_layoutMetrics.frame.size.height;
    }
    facebook::react::Float AutoLayoutViewComponentInstance::getHeight() { return m_layoutMetrics.frame.size.height; }
    facebook::react::Float AutoLayoutViewComponentInstance::getWidth() { return m_layoutMetrics.frame.size.width; }

    void AutoLayoutViewComponentInstance::onAppear() {
        fixLayout();
        fixFooter();
        getLocalRootArkUINode().markDirty();

        parentScrollView = getParentScrollView();
        if (enableInstrumentation && parentScrollView != nullptr) {
            auto scrollContainerSize = alShadow.horizontal ? parentScrollView->getLayoutMetrics().frame.size.width
                                                           : parentScrollView->getLayoutMetrics().frame.size.height;
            auto currentScrollOffset = alShadow.horizontal ? parentScrollView->getScrollViewMetrics().contentOffset.x
                                                           : parentScrollView->getScrollViewMetrics().contentOffset.y;

            auto startOffset = alShadow.horizontal ? getLeft() : getTop();
            auto endOffset = alShadow.horizontal ? getRight() : getBottom();

            auto distanceFromWindowStart = MAX(startOffset - currentScrollOffset, 0);
            auto distanceFromWindowEnd = MAX(currentScrollOffset + scrollContainerSize - endOffset, 0);
            alShadow.computeBlankFromGivenOffset(currentScrollOffset - startOffset,
                                                 distanceFromWindowStart,
                                                 distanceFromWindowEnd);
            emitBlankAreaEvent();
        }
    }

    void AutoLayoutViewComponentInstance::onPropsChanged(SharedConcreteProps const &props) {
        CppComponentInstance::onPropsChanged(props);
        horizontal = props->horizontal;
        alShadow.horizontal = props->horizontal;
        alShadow.scrollOffset = props->scrollOffset;
        alShadow.windowSize = props->windowSize;
        alShadow.renderOffset = props->renderAheadOffset;
        enableInstrumentation = props->enableInstrumentation;
        disableAutoLayout = props->disableAutoLayout;
        if (parentScrollView != nullptr) {
            onAppear();
        }
    }

    /** Sorts views by index and then invokes clearGaps which does the correction.
     * Performance: Sort is needed. Given relatively low number of views in RecyclerListView render tree this should be
     * a non issue.*/
    void AutoLayoutViewComponentInstance::fixLayout() {
        auto children = getChildren();
        if (children.size() > 1 && !disableAutoLayout) {
            std::vector<rnoh::CellContainerComponentInstance::Shared> childrenView;
            for (int i = 0; i < children.size(); i++) {
                auto cell = std::dynamic_pointer_cast<rnoh::CellContainerComponentInstance>(children[i]);
                childrenView.push_back(cell);
            }
            std::sort(childrenView.begin(), childrenView.end(),
                    [](auto &a, auto &b) { return a->getIndex() < b->getIndex(); });

            alShadow.offsetFromStart = alShadow.horizontal ? getLeft() : getTop();
            alShadow.clearGapsAndOverlaps(childrenView);
        }
    }

    /** Fixes footer position along with rest of the items */
    void AutoLayoutViewComponentInstance::fixFooter() {
        parentScrollView = getParentScrollView();
        if (disableAutoLayout || parentScrollView == nullptr) {
            return;
        }
        auto isAutoLayoutEndVisible = alShadow.horizontal
                                          ? getRight() <= parentScrollView->getLayoutMetrics().frame.size.width
                                          : getBottom() <= parentScrollView->getLayoutMetrics().frame.size.height;
        if (!isAutoLayoutEndVisible) {
            return;
        }
        auto autoLayoutParent = getParent().lock();
        auto footer = getFooter();
        auto diff = static_cast<facebook::react::Float>(getFooterDiff());
        if (diff == 0 || footer == nullptr || autoLayoutParent == nullptr) {
            return;
        }
        if (alShadow.horizontal) {
            auto footerLayoutMetrics = footer->getLayoutMetrics();
            footerLayoutMetrics.frame.origin.x += diff;
            footer->setLayout(footerLayoutMetrics);

            m_layoutMetrics.frame.size.width += diff;
            setLayout(m_layoutMetrics);

            auto parentLayoutMetrics = autoLayoutParent->getLayoutMetrics();
            parentLayoutMetrics.frame.size.width += diff;
            autoLayoutParent->setLayout(parentLayoutMetrics);
        } else {
            auto footerLayoutMetrics = footer->getLayoutMetrics();
            footerLayoutMetrics.frame.origin.y += diff;
            footer->setLayout(footerLayoutMetrics);

            m_layoutMetrics.frame.size.height += diff;
            setLayout(m_layoutMetrics);

            auto parentLayoutMetrics = autoLayoutParent->getLayoutMetrics();
            parentLayoutMetrics.frame.size.height += diff;
            autoLayoutParent->setLayout(parentLayoutMetrics);
        }
    }

    facebook::react::Float AutoLayoutViewComponentInstance::getFooterDiff() {
        if (getChildren().empty()) {
            alShadow.lastMaxBoundOverall = 0;
        } else if (getChildren().size() == 1) {
            auto firstChild = std::dynamic_pointer_cast<rnoh::CellContainerComponentInstance>(getChildren()[0]);
            alShadow.lastMaxBoundOverall = alShadow.horizontal ? firstChild->getRight() : firstChild->getBottom();
        }
        auto autoLayoutEnd = alShadow.horizontal ? getWidth() : getHeight();
        return alShadow.lastMaxBoundOverall - autoLayoutEnd;
    }

    std::shared_ptr<rnoh::CellContainerComponentInstance> AutoLayoutViewComponentInstance::getFooter() {
        for (auto const child : getChildren()) {
            auto childInstance = std::dynamic_pointer_cast<rnoh::CellContainerComponentInstance>(child);
            if (childInstance != nullptr && childInstance->getIndex() == -1) {
                return childInstance;
            }
        }
        return nullptr;
    }

    std::shared_ptr<rnoh::ScrollViewComponentInstance> AutoLayoutViewComponentInstance::getParentScrollView() {
        auto autoLayoutParent = getParent().lock();
        while (autoLayoutParent) {
            auto scrollView = std::dynamic_pointer_cast<rnoh::ScrollViewComponentInstance>(autoLayoutParent);
            if (scrollView) {
                return scrollView;
            }
            autoLayoutParent = autoLayoutParent->getParent().lock();
        }
        return nullptr;
    }

    void AutoLayoutViewComponentInstance::emitBlankAreaEvent() {
        AutoLayoutViewEventEmitter::OnBlankAreaEvent blankAreaEvent;
        blankAreaEvent.offsetStart = alShadow.blankOffsetAtStart / pixelDensity;
        blankAreaEvent.offsetEnd = alShadow.blankOffsetAtEnd / pixelDensity;
        m_eventEmitter->onBlankAreaEvent(blankAreaEvent);
    }
} // namespace rnoh
