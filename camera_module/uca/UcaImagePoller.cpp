
#include "UcaImagePoller.h"

#include <uca/uca-plugin-manager.h>


UcaImagePoller::UcaImagePoller(std::string const& name)
{
  GError* error=nullptr;
  UcaPluginManager *manager = uca_plugin_manager_new();
  mCamera = uca_plugin_manager_get_camera(manager, name.c_str(), &error, NULL);
  g_object_unref(manager);

  if (!mCamera || error != nullptr) {
    auto message = "Couldn't initialize camera " + name;

    if (error)
    {
      message += std::string(":") + error->message;
      g_error_free(error);
    }
    throw std::runtime_error(message);
  }

}

void UcaImagePoller::startAcquisition()
{

}

void UcaImagePoller::poll(bool toneMapping)
{

}

void UcaImagePoller::stopAcquisition()
{

}
