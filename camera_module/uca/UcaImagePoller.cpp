
#include "UcaImagePoller.h"

#include <uca/uca-plugin-manager.h>

namespace {
guint bytes_per_pixel (guint bits)
{
  if (bits==8||bits==16||bits==32)
    return bits / 8;
  throw std::runtime_error("Unsupported bit depth");
}

void check_error(GError* error, std::string const& context)
{
  if (error== nullptr)
    return;

  auto message = context + error->message;
  g_error_free(error);
  throw std::runtime_error(message);
}
}

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
  g_object_set (G_OBJECT (mCamera),
                "trigger-source",
                UCA_CAMERA_TRIGGER_SOURCE_AUTO,
                NULL);

  g_object_get (G_OBJECT (mCamera),
                "roi-width", &mWidth,
                "roi-height", &mHeight,
                "sensor-bitdepth", &mBits,
                NULL);


  GError* error=nullptr;
  uca_camera_start_recording(mCamera, &error);

  check_error(error, "Error starting camera ");

  mBuffer.resize(bytes_per_pixel(mBits)*mWidth*mHeight);
}

void UcaImagePoller::poll(bool toneMapping)
{
  GError* error=nullptr;
  uca_camera_grab (mCamera, mBuffer.data(), &error);
  check_error(error, "Error grabbing image ");
}

void UcaImagePoller::stopAcquisition()
{
  GError* error=nullptr;
  uca_camera_stop_recording (mCamera, &error);

  check_error(error, "Error stopping camera ");
}

UcaImagePoller::~UcaImagePoller()
{
  g_object_unref (mCamera);
}
