#pragma once

#include "RNOH/arkui/ArkUINode.h"
#include "RNOH/arkui/NativeNodeApi.h"
#include "FlashListStackNode.h"

namespace rnoh {
class FlashListStackNodeDelegate {
public:
    virtual ~FlashListStackNodeDelegate() = default;
    virtual void onClick(){};
    virtual void onHoverIn(){};
    virtual void onHoverOut(){};
};

class FlashListStackNode : public ArkUINode {
protected:
    FlashListStackNodeDelegate *m_stackNodeDelegate;

public:
    FlashListStackNode();
    ~FlashListStackNode() override;

    void insertChild(ArkUINode &child, std::size_t index);
    void addChild(ArkUINode &child);
    void removeChild(ArkUINode &child);
    void onNodeEvent(ArkUI_NodeEventType eventType, EventArgs &eventArgs) override;
    void onClick();
    void setStackNodeDelegate(FlashListStackNodeDelegate *stackNodeDelegate);
    FlashListStackNode &setAlign(int32_t align);
};

} // namespace rnoh