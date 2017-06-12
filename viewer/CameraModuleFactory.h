#pragma once
#include <vector>
#include <string>
#include <memory>
#include "AbstractImagePoller.h"

class CameraModuleFactory
{
public:
  using Factory = std::function<std::shared_ptr<AbstractImagePoller>()>;

  CameraModuleFactory();

  std::vector<std::string> getAvailableModules() const;
  std::shared_ptr<AbstractImagePoller> createModule(std::string const& name) const;
private:
  std::map<std::string, Factory> mFactories;
};


