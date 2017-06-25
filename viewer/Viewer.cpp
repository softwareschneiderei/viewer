#include "Viewer.h"
#include "ui_Viewer.h"
#include <QToolButton>
#include <QPushButton>
#include <UcaImagePoller.h>
#include "EmulatedImagePoller.h"

Viewer::Viewer(QWidget* parent) :
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

  connect(mUi->configure, &QPushButton::clicked, [this]{
    if (!mPoller)
    {
      return;
    }
    auto configurationWidget = mPoller->configure(this);
    if (!configurationWidget)
    {
      return;
    }
    configurationWidget->show();
  });

  mImageStatsLabel = new QLabel(this);
  statusBar()->addPermanentWidget(mImageStatsLabel);
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
    mPoller->start([this](AbstractImagePoller::Result result)
                   {
                     updateImage(result);
                   });
  }

}
void Viewer::updateImage(AbstractImagePoller::Result result)
{
  mUi->display->setImage(result.image);
  mImageStatsLabel->setText(QString("Min: %1, Max %2").arg(result.min).arg(result.max));
}
