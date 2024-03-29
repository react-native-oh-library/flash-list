#pragma once
#include "Props.h"
#include "RNOH/CppComponentInstance.h"
#include "RNOH/arkui/StackNode.h"
#include "ShadowNodes.h"

namespace rnoh {
    class CellContainerComponentInstance : public CppComponentInstance<facebook::react::CellContainerShadowNode> {
    private:
        StackNode m_stackNode;
        int index{-1};

    public:
        CellContainerComponentInstance(Context context);

        void onChildInserted(ComponentInstance::Shared const &childComponentInstance, std::size_t index) override;

        void onChildRemoved(ComponentInstance::Shared const &childComponentInstance) override;

        StackNode &getLocalRootArkUINode() override;

        void setIndex(int const &);
        int getIndex();

        void setLeft(facebook::react::Float const &);
        facebook::react::Float getLeft();
        void setTop(facebook::react::Float const &);
        facebook::react::Float getTop();
        void setRight(facebook::react::Float const &);
        facebook::react::Float getRight();
        void setBottom(facebook::react::Float const &);
        facebook::react::Float getBottom();
        void setHeight(facebook::react::Float const &);
        facebook::react::Float getHeight();
        void setWidth(facebook::react::Float const &);
        facebook::react::Float getWidth();

        void onPropsChanged(SharedConcreteProps const &props) override;
    };
} // namespace rnoh