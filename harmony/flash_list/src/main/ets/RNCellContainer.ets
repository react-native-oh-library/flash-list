import {
  Descriptor,
  ComponentBuilderContext,
  RNComponentFactory,
  RNOHContext,
  RNViewBase,
  ViewDynamicProps
} from 'rnoh'
import emitter from '@ohos.events.emitter';

export const CELL_CONTAINER_TYPE = "CellContainer" as const

export type CellContainerProps = ViewDynamicProps & {
  index: number
  right: number
  bottom: number
  top: number
  left: number
  height: number
  width: number
  backgroundColor: number[]
  borderColor: number[]
  // backgroundColor: number & number[]
  // borderColor: number & number[]
}

export type CellContainerDescriptor = Descriptor<typeof CELL_CONTAINER_TYPE, CellContainerProps>

@Component
export struct RNCellContainer {
  ctx: RNOHContext
  tag: number
  @BuilderParam buildCustomComponent: (componentBuilderContext: ComponentBuilderContext) => void
  @State descriptor: CellContainerDescriptor = {} as CellContainerDescriptor
  private unregisterDescriptorChangesListener?: () => void = undefined
  @State index: number = -1;
  @State top: number = 0;

  aboutToAppear() {
    this.descriptor = this.ctx.descriptorRegistry.getDescriptor<CellContainerDescriptor>(this.tag)
    this.unregisterDescriptorChangesListener = this.ctx.descriptorRegistry.subscribeToDescriptorChanges(this.tag,
      (newDescriptor) => {
        this.descriptor = (newDescriptor as CellContainerDescriptor)
      }
    )
    console.log(`[RNOH]: in RNCellContainer After descriptor: ${JSON.stringify(this.descriptor)}`)
    if (this.descriptor.props.top) {
      this.top = this.descriptor.props.top;
    }

    let innerEvent = {
      eventId: this.tag
    };

    // 收到eventId后的事件后执行该回调函数
    emitter.on(innerEvent, (data: any) => {
      console.log(`[RNOH]: in RNCellContainer emitterCallback TAG: ${JSON.stringify(this.tag)}`)
      console.log(`[RNOH]: in RNCellContainer emitterCallback DATA: ${JSON.stringify(data)}`)
      this.top = data.data.footerTop
      console.log(`[RNOH]: in RNCellContainer emitterCallback this.top: ${JSON.stringify(this.top)}`)
    });
  }

  aboutToDisappear() {
    this.unregisterDescriptorChangesListener?.()
  }

  setIndex(value: number): void {
    this.index = value;
  }

  getIndex(): number {
    console.log(`[RNOH]: in RNCellContainer index: ${JSON.stringify(this.index)}`)
    return this.index;
  }

  build() {
    RNViewBase({ ctx: this.ctx, tag: this.tag }) {
      Stack() {
        ForEach(this.descriptor.childrenTags, (tag) => {
          RNComponentFactory({ ctx: this.ctx, tag: tag, buildCustomComponent: this.buildCustomComponent })
        }, tag => tag)
      }
      .width("100%")
      .height("100%")
      .zIndex(this.getIndex())
    }
  }
}

