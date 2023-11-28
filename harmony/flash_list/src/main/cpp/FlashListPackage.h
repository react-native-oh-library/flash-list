#include "ComponentDescriptors.h"
#include "RNOH/Package.h"
#include "FlashListViewNapiBinder.h"
#include "FlashListViewJSIBinder.h"
#include "FlashListEventEmitRequestHandler.h"

using namespace rnoh;
using namespace facebook;

class FlashListPackage : public Package {
  public: 
    FlashListPackage(Package::Context ctx) : Package(ctx) {}
    
    std::vector<facebook::react::ComponentDescriptorProvider> createComponentDescriptorProviders() override {
      return {
        facebook::react::concreteComponentDescriptorProvider<facebook::react::AutoLayoutViewComponentDescriptor>(),
        facebook::react::concreteComponentDescriptorProvider<facebook::react::CellContainerComponentDescriptor>(),
      };
    }
    ComponentJSIBinderByString createComponentJSIBinderByName() override {
      return {
        {"AutoLayoutView", std::make_shared<FlashListViewJSIBinder>()},
        {"CellContainer", std::make_shared<FlashListViewJSIBinder>()}
      };
    };
    
    ComponentNapiBinderByString createComponentNapiBinderByName() override {
      return {
        {"AutoLayoutView", std::make_shared<FlashListViewNapiBinder>()},
        {"CellContainer", std::make_shared<FlashListViewNapiBinder>()}
      };
    };
    
    EventEmitRequestHandlers createEventEmitRequestHandlers() override {
      return {std::make_shared<FlashListEventEmitRequestHandler>()};
    };
};