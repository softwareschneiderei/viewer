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

  for (auto const &moduleName : mCameraModuleFactory.getAvailableModules())
  {
    mUi->module->addItem(moduleName.c_str(), QString(moduleName.c_str()));
  }

  if (!mCameraModuleFactory.getAvailableModules().empty())
  {
    mPlaybackController.change(mCameraModuleFactory.createModule(mCameraModuleFactory.getAvailableModules().front()));
  }

  auto currentIndexChanged = static_cast<void (QComboBox::*)(QString const &)>(&QComboBox::currentIndexChanged);
  connect(mUi->module, currentIndexChanged, [this](QString module)
  {
    mPlaybackController.change(mCameraModuleFactory.createModule(module.toStdString()));
    mUi->display->setImage({});
  });

  connect(mUi->configure, &QPushButton::clicked, [this]
  {
    mPlaybackController.configure(this);
  });

  connect(mUi->play, &QToolButton::clicked, [this]
  {
    mPlaybackController.start();
  });
  connect(mUi->stop, &QToolButton::clicked, [this]
  {
    mPlaybackController.stop();
  });

  mImageStatsLabel = new QLabel(this);
  statusBar()->addPermanentWidget(mImageStatsLabel);

  mPlaybackController.setCallback([this](PlaybackController::TimedResult result)
                                  {
                                    updateImage(result);
                                  });
}

Viewer::~Viewer()
{
}

void Viewer::updateImage(PlaybackController::TimedResult result)
{
  auto fps = result.frameTime.count() > 0.0 ? 1.0 / result.frameTime.count() : 0.0;
  auto ms = result.frameTime.count() * 1000.0;
  mUi->display->setImage(result.image);
  mImageStatsLabel->setText(QString("Frametime: %1 ms, FPS: %2, Min: %3, Max %4")
                              .arg(ms, 4, 'f', 1)
                              .arg(fps, 4, 'f', 1)
                              .arg(result.min).arg(result.max));
}
