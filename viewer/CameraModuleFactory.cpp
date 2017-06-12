#include "CameraModuleFactory.h"
#include "UcaImagePoller.h"
#include "EmulatedImagePoller.h"

CameraModuleFactory::CameraModuleFactory()
{
  //mFactories["epics"] = [] {return std::make_shared<EpicsImagePoller>(device.toStdString(), serial)};
  //mPoller = std::make_shared<EmulatedImagePoller>();
  mFactories["uca"] = [] {return std::make_shared<UcaImagePoller>("mock");};
  mFactories["emulated"] = [] {return std::make_shared<EmulatedImagePoller>();};

}

std::vector<std::string> CameraModuleFactory::getAvailableModules() const
{
  auto result = std::vector<std::string>();

  for (auto factory : mFactories)
  {
    result.push_back(factory.first);
  }

  return result;
}

std::shared_ptr<AbstractImagePoller> CameraModuleFactory::createModule(std::string const &name) const
{
  return mFactories.at(name)();
}
