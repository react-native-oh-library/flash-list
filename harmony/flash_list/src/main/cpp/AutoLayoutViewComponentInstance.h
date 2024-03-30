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
        int getFooterDiff();
        void onAppear() override;
        std::shared_ptr<rnoh::CellContainerComponentInstance> getFooter();
        std::shared_ptr<rnoh::ScrollViewComponentInstance> getParentScrollView();
        void emitBlankAreaEvent() override;
    };
} // namespace rnoh

#endif // HARMONY_AUTOLAYOUTVIEWCOMPONENTINSTANCE_H
