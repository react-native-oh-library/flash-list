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

#ifndef HARMONY_AUTOLAYOUTVIEWCOMPONENTINSTANCE_H
#define HARMONY_AUTOLAYOUTVIEWCOMPONENTINSTANCE_H

#include "RNOH/CppComponentInstance.h"
#include "RNOHCorePackage/ComponentInstances/ScrollViewComponentInstance.h"
#include "AutoLayoutShadow.h"
#include "AutoLayoutNode.h"
#include "ShadowNodes.h"

namespace rnoh {
    class AutoLayoutViewComponentInstance : public CppComponentInstance<facebook::react::AutoLayoutViewShadowNode>,
                                            public AutoLayoutNodeDelegate {
    private:
        AutoLayoutNode m_autoLayoutNode;
        AutoLayoutShadow alShadow{AutoLayoutShadow()};
        bool horizontal{false};

        bool enableInstrumentation{false};
        bool disableAutoLayout{false};
        Float pixelDensity{1.0};

        std::shared_ptr<rnoh::ScrollViewComponentInstance> parentScrollView;

    public:
        AutoLayoutViewComponentInstance(Context context);

        void onChildInserted(ComponentInstance::Shared const &childComponentInstance, std::size_t index) override;

        void onChildRemoved(ComponentInstance::Shared const &childComponentInstance) override;

        AutoLayoutNode &getLocalRootArkUINode() override;

        void onPropsChanged(SharedConcreteProps const &props) override;
        void getNapiProps(facebook::react::Props::Shared props);
        void finalizeUpdates() override;

        facebook::react::Float getLeft();
        facebook::react::Float getTop();
        facebook::react::Float getRight();
        facebook::react::Float getBottom();
        facebook::react::Float getHeight();
        facebook::react::Float getWidth();

        void fixLayout();
        void fixFooter();
        facebook::react::Float getFooterDiff();
        void onAppear() override;
        std::shared_ptr<rnoh::CellContainerComponentInstance> getFooter();
        std::shared_ptr<rnoh::ScrollViewComponentInstance> getParentScrollView();
        void emitBlankAreaEvent() override;
    };
} // namespace rnoh

#endif // HARMONY_AUTOLAYOUTVIEWCOMPONENTINSTANCE_H
