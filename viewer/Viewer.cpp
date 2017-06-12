#include "Viewer.h"
#include "ui_Viewer.h"
#include <QToolButton>
#include <UcaImagePoller.h>
#include "EmulatedImagePoller.h"

Viewer::Viewer(QString device, int serial, QWidget* parent) :
  QMainWindow(parent),
  mUi(new Ui::Viewer)
{
  //mPoller = std::make_shared<EpicsImagePoller>(device.toStdString(), serial);
  //mPoller = std::make_shared<EmulatedImagePoller>();

  mUi->setupUi(this);


  for (auto const& moduleName : mCameraModuleFactory.getAvailableModules())
  {
    mUi->module->addItem(moduleName.c_str(), QString(moduleName.c_str()));
  }

  auto currentIndexChanged = static_cast<void(QComboBox::*)(QString const&)>(&QComboBox::currentIndexChanged);
  connect(mUi->module, currentIndexChanged, [this](QString module)
  {
    changePoller(mCameraModuleFactory.createModule(module.toStdString()));
  });

  mImageStatsLabel = new QLabel(this);
  statusBar()->addPermanentWidget(mImageStatsLabel);

  connect(mUi->tonemapping, &QToolButton::clicked, [this](bool checked)
  {
    mPoller->setAutoLeveling(checked);
  });

}

Viewer::~Viewer()
{
  changePoller(nullptr);
}

void Viewer::changePoller(std::shared_ptr<AbstractImagePoller> poller)
{
  if (mPoller)
  {
    mPoller->stop();
  }

  mPoller = poller;

  if (mPoller)
  {
    mPoller->start([this](EpicsImagePoller::Result result)
                   {
                     updateImage(result);
                   });
  }

}
void Viewer::updateImage(EpicsImagePoller::Result result)
{
  mUi->display->setImage(result.image);
  mImageStatsLabel->setText(QString("Min: %1, Max %2").arg(result.min).arg(result.max));
}
