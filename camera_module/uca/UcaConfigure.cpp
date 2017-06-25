
#include <QtWidgets/QMessageBox>
#include "UcaConfigure.h"
#include "ui_UcaConfigure.h"
#include "UcaTableModel.h"

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
    mUi->cameraSelector->addItem(camera.c_str(), {});
  }

  connect(mUi->cameraSelector, static_cast<void(QComboBox::*)(QString const&)>(&QComboBox::activated),
          [=](QString const& camera){ onCameraSelected(camera); });


}

UcaConfigure::~UcaConfigure()
{

}

void UcaConfigure::onCameraSelected(QString const& name)
{
  GError* error=nullptr;
  auto camera = uca_plugin_manager_get_camera(mManager, name.toUtf8().data(), &error, NULL);


  if (!camera || error != nullptr) {
    auto message = "Couldn't initialize camera " + name.toStdString();

    if (error)
    {
      message += std::string(":") + error->message;
      g_error_free(error);
    }
    QMessageBox::critical(this, "Error selecting uca camera", message.c_str());
    return;
  }

  mUi->properties->setModel(new UcaTableModel(camera));
  mUi->properties->setColumnWidth(0, 200);
}
