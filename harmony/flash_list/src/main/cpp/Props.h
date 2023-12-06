#pragma once

#include <jsi/jsi.h>
#include <react/renderer/components/view/ViewProps.h>
#include <react/renderer/core/PropsParserContext.h>

namespace facebook {
namespace react {

class JSI_EXPORT AutoLayoutViewProps final : public ViewProps {
  public:
    AutoLayoutViewProps() = default;
    AutoLayoutViewProps(const PropsParserContext& context, const AutoLayoutViewProps &sourceProps, const RawProps &rawProps);

#pragma mark - Props

  bool horizontal{false};
  facebook::react::Float scrollOffset{0.0};
  facebook::react::Float windowSize{0.0};
  facebook::react::Float renderAheadOffset{0.0};
  bool enableInstrumentation{false};
  bool disableAutoLayout{false};
};

class JSI_EXPORT CellContainerProps final : public ViewProps {
  public:
    CellContainerProps() = default;
    CellContainerProps(const PropsParserContext& context, const CellContainerProps &sourceProps, const RawProps &rawProps);

#pragma mark - Props

  int index{0};
};

} // namespace react
} // namespace facebook