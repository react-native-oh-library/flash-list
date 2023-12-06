#include "Props.h"
#include <react/renderer/core/PropsParserContext.h>
#include <react/renderer/core/propsConversions.h>

namespace facebook {
namespace react {

AutoLayoutViewProps::AutoLayoutViewProps(
  const PropsParserContext &context,
  const AutoLayoutViewProps &sourceProps,
  const RawProps &rawProps): ViewProps(context, sourceProps, rawProps),

  horizontal(convertRawProp(context, rawProps, "horizontal", sourceProps.horizontal, {false})),
  scrollOffset(convertRawProp(context, rawProps, "scrollOffset", sourceProps.scrollOffset, {0.0})),
  windowSize(convertRawProp(context, rawProps, "windowSize", sourceProps.windowSize, {0.0})),
  renderAheadOffset(convertRawProp(context, rawProps, "renderAheadOffset", sourceProps.renderAheadOffset, {0.0})),
  enableInstrumentation(convertRawProp(context, rawProps, "enableInstrumentation", sourceProps.enableInstrumentation, {false})),
  disableAutoLayout(convertRawProp(context, rawProps, "disableAutoLayout", sourceProps.disableAutoLayout, {false})) {}

CellContainerProps::CellContainerProps(
  const PropsParserContext &context,
  const CellContainerProps &sourceProps,
  const RawProps &rawProps): ViewProps(context, sourceProps, rawProps),
  
  index(convertRawProp(context, rawProps, "index", sourceProps.index, {0})) {}

} // namespace react
} // namespace facebook