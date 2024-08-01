#include "RNOH/PackageProvider.h"
#include "FlashListPackage.h"

using namespace rnoh;

std::vector<std::shared_ptr<Package>> PackageProvider::getPackages(
    Package::Context ctx) {
  return {
        std::make_shared<FlashListPackage>(ctx),
  };
}