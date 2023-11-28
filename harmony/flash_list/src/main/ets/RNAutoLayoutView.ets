import {
  Descriptor,
  ComponentBuilderContext,
  RNComponentFactory,
  RNOHContext,
  RNViewBase,
  ViewDynamicProps
} from 'rnoh'

export type ViewDescriptor = Descriptor<"View", ViewDynamicProps & {
  top: number
  left: number
  height: number
  width: number
}>

export type ScrollViewProps = ViewDynamicProps & {
  scrollEnabled?: boolean
  contentOffsetX: number
  contentOffsetY: number
  bounces: boolean
  alwaysBounceHorizontal: boolean
  showsHorizontalScrollIndicator: boolean
  showsVerticalScrollIndicator: boolean
  top: number
  left: number
  height: number
  width: number
}

export type ScrollViewState = {
  contentOffsetX: number
  contentOffsetY: number
  contentSizeWidth: number
  contentSizeHeight: number
}

export type ScrollViewDescriptor = Descriptor<"ScrollView", ScrollViewProps, ScrollViewState>

import { RNCellContainer, CellContainerDescriptor, CellContainerProps } from "./RNCellContainer"
import { AutoLayoutShadow } from "./RNAutoLayoutShadow"
import emitter from '@ohos.events.emitter';

export type AutoLayoutViewProps = ViewDynamicProps & {
  scrollOffset: number
  windowSize: number
  renderAheadOffset: number
  horizontal: number
  enableInstrumentation: boolean
  disableAutoLayout: boolean
  top: number
  left: number
  height: number
  width: number
}

export const FLASH_LIST_TYPE = "AutoLayoutView" as const


export type AutoLayoutViewDescriptor = Descriptor<typeof FLASH_LIST_TYPE, ViewDynamicProps & {
  scrollOffset: number
  windowSize: number
  renderAheadOffset: number
  horizontal: number
  enableInstrumentation: boolean
  disableAutoLayout: boolean
  top: number
  left: number
  height: number
  width: number
}>

@Component
export struct RNAutoLayoutView {
  ctx: RNOHContext
  tag: number
  @BuilderParam buildCustomComponent: (componentBuilderContext: ComponentBuilderContext) => void
  @State descriptor: AutoLayoutViewDescriptor = {} as AutoLayoutViewDescriptor
  private unregisterDescriptorChangesListener?: () => void = undefined
  private alShadow: AutoLayoutShadow = new AutoLayoutShadow();
  private children: number[];
  @State top: number = 0
  @State left: number = 0
  private footer: CellContainerDescriptor;

  aboutToAppear() {
    this.descriptor = this.ctx.descriptorRegistry.getDescriptor<AutoLayoutViewDescriptor>(this.tag)
    this.unregisterDescriptorChangesListener = this.ctx.descriptorRegistry.subscribeToDescriptorChanges(this.tag,
      (newDescriptor) => {
        this.descriptor = (newDescriptor as AutoLayoutViewDescriptor)
      }
    )
    this.top = this.descriptor.props.top
    this.left = this.descriptor.props.left
    console.log(`[RNOH]: in RNAutoLayoutView: ${JSON.stringify(this.descriptor)}`)

    for (let index = 0; index < this.descriptor.childrenTags.length; index++) {
      const childTag: Descriptor = this.ctx.descriptorRegistry.getDescriptor(this.descriptor.childrenTags[index])
      console.log(`[RNOH]: in RNAutoLayoutView ForEach childrenTags: ${JSON.stringify(childTag)}`)
    }
    console.log(`[RNOH]: in RNAutoLayoutView is horizontal: ${this.descriptor.props.horizontal}`)
    this.alShadow.horizontal = this.descriptor.props.horizontal === 1
    this.alShadow.scrollOffset = this.descriptor.props.scrollOffset
    this.alShadow.windowSize = this.descriptor.props.windowSize
    this.alShadow.renderOffset = this.descriptor.props.renderAheadOffset

    this.updateAutoLayout()
    this.emitBlankAreaEvent();
    const footerTag = this.getFooterTag();
    this.footer = this.ctx.descriptorRegistry.getDescriptor(footerTag) as CellContainerDescriptor;
    this.unregisterDescriptorChangesListener = this.ctx.descriptorRegistry.subscribeToDescriptorChanges(this.tag,
      (newDescriptor) => {
        this.descriptor = (newDescriptor as AutoLayoutViewDescriptor)
        console.log(`[RNOH]: in RNAutoLayoutView AboutToAppear After Listener descriptor: ${JSON.stringify(this.descriptor)}`)
      }
    )

    const autoLayoutParentTag = this.getParentViewTag();
    const autoLayoutParentDescriptor: ViewDescriptor = this.ctx.descriptorRegistry.getDescriptor(autoLayoutParentTag) as ViewDescriptor
    console.log(`[RNOH]: in RNAutoLayoutView aboutToAppear autoLayoutParentDescriptor: ${JSON.stringify(autoLayoutParentDescriptor)}`)
    autoLayoutParentDescriptor.props.height = this.descriptor.props.height + this.footer?.props.height;
    autoLayoutParentDescriptor.layoutMetrics.frame.size.height = autoLayoutParentDescriptor.props.height;
    console.log(`[RNOH]: in RNAutoLayoutView aboutToAppear After autoLayoutParentDescriptor: ${JSON.stringify(autoLayoutParentDescriptor)}`)

    const autoLayoutGrandparentTag = this.getAutoLayoutGrandparentTag();
    const autoLayoutGrandparentDescriptor: ViewDescriptor = this.ctx.descriptorRegistry.getDescriptor(autoLayoutGrandparentTag) as ViewDescriptor;
    console.log(`[RNOH]: in RNAutoLayoutView aboutToAppear autoLayoutGrandparentDescriptor: ${JSON.stringify(autoLayoutGrandparentDescriptor)}`)
    autoLayoutGrandparentDescriptor.props.height = this.descriptor.props.height + this.footer?.props.height;
    autoLayoutGrandparentDescriptor.layoutMetrics.frame.size.height = this.descriptor.props.height + this.footer?.props.height;
    console.log(`[RNOH]: in RNAutoLayoutView aboutToAppear After autoLayoutGrandparentDescriptor: ${JSON.stringify(autoLayoutGrandparentDescriptor)}`)

    const parentScrollViewTag = this.getParentScrollViewTag();
    const scrollViewDescriptor: ScrollViewDescriptor = this.ctx.descriptorRegistry.getDescriptor(parentScrollViewTag) as ScrollViewDescriptor
    console.log(`[RNOH]: in RNAutoLayoutView aboutToAppear scrollViewDescriptor: ${JSON.stringify(scrollViewDescriptor)}`)
    scrollViewDescriptor.props.height = this.descriptor.props.height + this.footer?.props.height;
    scrollViewDescriptor.state.contentSizeHeight = this.descriptor.props.height + this.footer?.props.height;
    console.log(`[RNOH]: in RNAutoLayoutView aboutToAppear After scrollViewDescriptor: ${JSON.stringify(scrollViewDescriptor)}`)
  }

  aboutToDisappear() {
    console.info('RNAutoLayoutView aboutToDisappear');
    this.unregisterDescriptorChangesListener?.();
  }

  fixLayout(): void {
    console.log(`[RNOH]: in RNAutoLayoutView fixLayout: start `)

    if (this.children.length > 1 && !this.descriptor.props.disableAutoLayout) {
      console.log(`[RNOH]: in RNAutoLayoutView fixLayout children: ` + this.children)
      this.alShadow.offsetFromStart = this.alShadow.horizontal ? this.descriptor.props.left : this.descriptor.props.top;
      this.alShadow.clearGapsAndOverlaps(this.children, this.ctx);
    }
  }

  updateAutoLayout() {
    this.children = this.ctx.descriptorRegistry.getDescriptor(this.tag).childrenTags;
    console.log(`[RNOH]: in RNAutoLayoutView descriptor: ${JSON.stringify(this.descriptor)}`)
    this.fixLayout();
    this.fixFooter();

    const parentScrollViewTag = this.getParentScrollViewTag();
    let scrollViewDescriptor: ScrollViewDescriptor = this.ctx.descriptorRegistry.getDescriptor(parentScrollViewTag) as ScrollViewDescriptor

    let scrollContainerSize: number = this.alShadow.horizontal ? scrollViewDescriptor.props.width : scrollViewDescriptor.props.height

    let scrollOffset: number = this.alShadow.horizontal ? scrollViewDescriptor.props.contentOffsetX : scrollViewDescriptor.props.contentOffsetY

    let startOffset: number = this.alShadow.horizontal ? this.descriptor.props.left : this.descriptor.props.top
    let endOffset: number = this.alShadow.horizontal ? (this.descriptor.props.left + this.descriptor.props.width) : (this.descriptor.props.top + this.descriptor.props.height)

    let distanceFromWindowStart: number = Math.max(startOffset - scrollOffset, 0)
    let distanceFromWindowEnd: number = Math.max(scrollOffset + scrollContainerSize - endOffset, 0)
    this.descriptor.props.windowSize = this.descriptor.props.height

    this.alShadow.computeBlankFromGivenOffset(scrollOffset, distanceFromWindowStart, distanceFromWindowEnd)

    const firstChildTag = this.children[0];
    const firstChildDescriptor: CellContainerDescriptor = this.ctx.descriptorRegistry.getDescriptor(firstChildTag) as CellContainerDescriptor;
    console.log(`[RNOH]: in RNAutoLayoutView Final firstChildDescriptor: ${JSON.stringify(firstChildDescriptor)}`)
    console.log(`[RNOH]: in RNAutoLayoutView Final descriptor: ${JSON.stringify(this.descriptor)}`)
    return this.children;
  }

  fixFooter(): void {
    const parentScrollViewTag = this.getParentScrollViewTag();
    if (this.descriptor.props.disableAutoLayout || parentScrollViewTag == null) {
      return;
    }
    let scrollViewDescriptor: ScrollViewDescriptor = this.ctx.descriptorRegistry.getDescriptor(parentScrollViewTag) as ScrollViewDescriptor
    console.log(`[RNOH]: in RNAutoLayoutView fixFooter scrollViewDescriptor: ${JSON.stringify(scrollViewDescriptor)}`)
    const isAutoLayoutEndVisible = this.alShadow.horizontal ? (this.descriptor.props.left + this.descriptor.props.width) <= scrollViewDescriptor.props.width : (this.descriptor.props.top + this.descriptor.props.height) <= scrollViewDescriptor.props.height;
    if (!isAutoLayoutEndVisible) {
      return;
    }
    let autoLayoutParentTag = this.getParentViewTag();
    let autoLayoutParentDescriptor: ViewDescriptor = this.ctx.descriptorRegistry.getDescriptor(autoLayoutParentTag) as ViewDescriptor
    console.log(`[RNOH]: in RNAutoLayoutView fixFooter autoLayoutParentDescriptor: ${JSON.stringify(autoLayoutParentDescriptor)}`)

    // footer AutoLayoutView的兄弟CellContainer节点
    const footerTag = this.getFooterTag();
    const footer = this.ctx.descriptorRegistry.getDescriptor(footerTag) as CellContainerDescriptor;
    const diff = this.getFooterDiff();
    if (diff === 0 || footer == null || autoLayoutParentTag == null) {
      return;
    }

    if (this.alShadow.horizontal) {
      this.footer.props.left += diff;
      this.descriptor.props.left += diff;
      autoLayoutParentDescriptor.props.left += diff;
    } else {
      // TODO diff 待确定
      // this.descriptor.props.top = this.descriptor.props.top - 100;
      this.footer.props.top = this.descriptor.props.height;
      this.descriptor.props.height = this.descriptor.props.height - diff;
      autoLayoutParentDescriptor.props.height = this.descriptor.props.height + this.footer.props.height
      // + this.headerDiff
      autoLayoutParentDescriptor.layoutMetrics.frame.size.height = autoLayoutParentDescriptor.props.height;
      console.log(`[RNOH]: in RNAutoLayoutView fixFooter After autoLayoutParentDescriptor: ${JSON.stringify(autoLayoutParentDescriptor)}`)
      autoLayoutParentDescriptor = JSON.parse(JSON.stringify(autoLayoutParentDescriptor));
    }
  }

  getFooterDiff(): number {
    if (this.children.length === 0) {
      this.alShadow.lastMaxBoundOverall = 0;
    } else {
      const lastChildTag = this.children[this.children.length - 1];
      const lastChildDescriptor: CellContainerDescriptor = this.ctx.descriptorRegistry.getDescriptor(lastChildTag) as CellContainerDescriptor;
      this.alShadow.lastMaxBoundOverall = this.alShadow.horizontal ? (lastChildDescriptor.props.left + lastChildDescriptor.props.width) : (lastChildDescriptor.props.top + lastChildDescriptor.props.height)
    }
    const autoLayoutEnd = this.alShadow.horizontal ? this.descriptor.props.width : this.descriptor.props.height;
    console.log(`[RNOH]: in RNAutoLayoutView fixFooter getFooterDiff lastMaxBoundOverall: ${JSON.stringify(this.alShadow.lastMaxBoundOverall)}`)
    console.log(`[RNOH]: in RNAutoLayoutView fixFooter getFooterDiff autoLayoutEnd: ${JSON.stringify(autoLayoutEnd)}`)
    return autoLayoutEnd - this.alShadow.lastMaxBoundOverall;
  }

  getFooterTag(): number | null {
    let ParentTag: number = this.descriptor.parentTag as number;
    let ParentDescriptor: ViewDescriptor = this.ctx.descriptorRegistry.getDescriptor(ParentTag) as ViewDescriptor;
    let ParentChildrenTags: number[] = ParentDescriptor.childrenTags;
    for (let i = 0; i < ParentChildrenTags.length; i++) {
      const viewTag = ParentChildrenTags[i];
      const view: CellContainerProps = this.ctx.descriptorRegistry.getDescriptor(viewTag).props as CellContainerProps;
      if (view.index === -1) {
        console.log(`[RNOH]: in RNAutoLayoutView fixFooter getFooterTag: ${JSON.stringify(viewTag)}`)
        console.log(`[RNOH]: in RNAutoLayoutView fixFooter getFooter: ${JSON.stringify(view)}`)
        return viewTag;
      }
    }
    return null;
  }

  getAutoLayoutGrandparentTag(): number {
    let autoLayoutParentTag: number = this.descriptor.parentTag as number;
    let autoLayoutParentDescriptor: ViewDescriptor = this.ctx.descriptorRegistry.getDescriptor(autoLayoutParentTag) as ViewDescriptor;
    let autoLayoutGrandparentTag: number = autoLayoutParentDescriptor.parentTag as number;
    let autoLayoutGrandparentDescriptor: ViewDescriptor = this.ctx.descriptorRegistry.getDescriptor(autoLayoutGrandparentTag) as ViewDescriptor;
    console.log(`[RNOH]: in RNAutoLayoutView getAutoLayoutGrandparentTag autoLayoutGrandparentDescriptor: ${JSON.stringify(autoLayoutGrandparentDescriptor)}`)
    return autoLayoutGrandparentTag;
  }

  getParentScrollViewTag(): number {
    let autoLayoutParentTag: number = this.descriptor.parentTag as number;
    let autoLayoutParentDescriptor: ViewDescriptor = this.ctx.descriptorRegistry.getDescriptor(autoLayoutParentTag) as ViewDescriptor;
    let autoLayoutGrandparentTag: number = autoLayoutParentDescriptor.parentTag as number;
    let autoLayoutGrandparentDescriptor: ViewDescriptor = this.ctx.descriptorRegistry.getDescriptor(autoLayoutGrandparentTag) as ViewDescriptor;
    let scrollViewTag: number = autoLayoutGrandparentDescriptor.parentTag as number;
    let scrollViewDescriptor: Descriptor = this.ctx.descriptorRegistry.getDescriptor(scrollViewTag)
    console.log(`[RNOH]: in RNAutoLayoutView getParentScrollViewTag scrollViewDescriptor: ${JSON.stringify(scrollViewDescriptor)}`)
    return scrollViewTag;
  }

  getParentViewTag(): number {
    let autoLayoutParentTag: number = this.descriptor.parentTag as number;
    let autoLayoutParentDescriptor: ScrollViewDescriptor = this.ctx.descriptorRegistry.getDescriptor(autoLayoutParentTag) as ScrollViewDescriptor;
    return autoLayoutParentTag;
  }

  emitBlankAreaEvent() {
    this.ctx.rnInstanceManager.emitComponentEvent(
      this.descriptor.tag,
      FLASH_LIST_TYPE,
      {
        offsetStart: this.alShadow.blankOffsetAtStart,
        offsetEnd: this.alShadow.blankOffsetAtEnd,
        blankArea: Math.max(this.alShadow.blankOffsetAtStart, this.alShadow.blankOffsetAtEnd)
      }
    )
  }

  build() {
    RNViewBase({ ctx: this.ctx, tag: this.tag }) {
      ForEach(this.descriptor.childrenTags, (childrenTag) => {
        Stack() {
          RNComponentFactory({ ctx: this.ctx, tag: childrenTag, buildCustomComponent: this.buildCustomComponent })
        }
        .width("100%")
        .height("100%")
      }, childrenTag => childrenTag)
    }
  }
}