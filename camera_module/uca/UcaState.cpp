

#include "UcaState.h"
#include <stdexcept>

namespace
{

void checkError(GError* error, std::string const &context)
{
  if (error== nullptr)
    return;

  auto message = context + error->message;
  g_error_free(error);
  throw std::runtime_error(message);
}

}

UcaState::UcaState()
{
  mManager = uca_plugin_manager_new();
}

UcaState::~UcaState()
{
  if (mCamera)
  {
    g_object_unref(mCamera);
    mCamera = nullptr;
  }

  g_object_unref(mManager);
}

UcaPluginManager* UcaState::getManager()
{
  return mManager;
}

UcaCamera* UcaState::getCamera() const
{
  return mCamera;
}

void UcaState::setCameraByName(std::string const& cameraName)
{
  if (mCamera)
  {
    g_object_unref(mCamera);
    mCamera = nullptr;
  }

  mCamera = createCameraByName(cameraName);
  mCameraName = cameraName;
}

UcaCamera* UcaState::createCameraByName(std::string const& cameraName) const
{
  GError* error=nullptr;
  auto camera = uca_plugin_manager_get_camera(mManager, cameraName.c_str(), &error, NULL);


  if (!camera || error != nullptr) {
    auto message = "Couldn't initialize camera " + cameraName;

    if (error)
    {
      message += std::string(":") + error->message;
      g_error_free(error);
    }
    if (camera)
    {
      g_object_unref(camera);
    }
    throw std::runtime_error(message);
  }

  return camera;
}

std::vector<std::string> UcaState::getAvailableCameras() const
{
  auto types = uca_plugin_manager_get_available_cameras (mManager);

  if (!types)
  {
    return {};
  }

  auto result = std::vector<std::string>();

  for (GList* node = g_list_first (types); node; node = g_list_next (node))
  {
    auto name = static_cast<gchar const*>(node->data);
    result.push_back(name);
  }

  return result;
}


std::string UcaState::getCameraName() const
{
  return mCameraName;
}


void UcaState::setAutoLevel(bool rhs)
{
  mAutoLevel = rhs;
}


bool UcaState::getAutoLevel() const
{
  return mAutoLevel;
}
