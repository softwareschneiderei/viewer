
#include "UcaConfigure.h"
#include "ui_UcaConfigure.h"

namespace
{

std::vector<std::string> getAvailableCameras(UcaPluginManager* manager)
{
  auto types = uca_plugin_manager_get_available_cameras (manager);

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

}
UcaConfigure::UcaConfigure(UcaPluginManager* manager, QWidget* parent)
: QDialog(parent)
, mUi(new Ui::UcaConfigureDialog)
, mManager(manager)
{
  mUi->setupUi(this);

  auto cameras = getAvailableCameras(manager);

  for (auto const& camera : cameras)
  {
    mUi->availableCameras->addItem(camera.c_str(), {});
  }

}

UcaConfigure::~UcaConfigure()
{

}
