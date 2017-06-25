
#include <QtWidgets/QMessageBox>
#include "UcaConfigure.h"
#include "ui_UcaConfigure.h"
#include "UcaTableModel.h"
#include "UcaState.h"

namespace
{


}
UcaConfigure::UcaConfigure(std::shared_ptr<UcaState> state, QWidget* parent)
: QDialog(parent)
, mUi(new Ui::UcaConfigureDialog)
, mState(std::move(state))
{
  mUi->setupUi(this);

  auto cameras = mState->getAvailableCameras();

  for (auto const& camera : cameras)
  {
    mUi->cameraSelector->addItem(camera.c_str(), {});
  }

  connect(mUi->cameraSelector, static_cast<void(QComboBox::*)(QString const&)>(&QComboBox::activated),
          [=](QString const& camera){ onCameraSelected(camera); });

  setupTableModel();
}

UcaConfigure::~UcaConfigure()
{

}

void UcaConfigure::onCameraSelected(QString const& name)
{
  try
  {
    mState->setCameraByName(name.toStdString());
  }
  catch(std::exception const& e)
  {
    QMessageBox::critical(this, "Error selecting uca camera", e.what());
    return;
  }

  setupTableModel();
}

void UcaConfigure::setupTableModel() const
{
  mUi->properties->setModel(new UcaTableModel(mState->getCamera()));
  mUi->properties->setColumnWidth(0, 200);
  mUi->properties->setColumnWidth(1, 200);
}
