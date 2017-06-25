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
    mPlaybackController.configure(this);
  });

  connect(mUi->play, &QToolButton::clicked, [this] {mPlaybackController.start();});
  connect(mUi->stop, &QToolButton::clicked, [this] {mPlaybackController.stop();});

  mImageStatsLabel = new QLabel(this);
  statusBar()->addPermanentWidget(mImageStatsLabel);

  mPlaybackController.setCallback([this](AbstractImagePoller::Result result)
  {
    updateImage(result);
  });
}

Viewer::~Viewer()
{
  changePoller(nullptr);
}

void Viewer::changePoller(std::shared_ptr<AbstractImagePoller> poller)
{
  mPlaybackController.change(std::move(poller));
}
void Viewer::updateImage(AbstractImagePoller::Result result)
{
  mUi->display->setImage(result.image);
  mImageStatsLabel->setText(QString("Min: %1, Max %2").arg(result.min).arg(result.max));
}
