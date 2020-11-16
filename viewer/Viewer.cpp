#include "Viewer.h"
#include "EmulatedImagePoller.h"
#include "ui_Viewer.h"
#include <QPushButton>
#include <QToolButton>
#include <QtCore/QSettings>
#include <QtWidgets/QMessageBox>
#include <UcaImagePoller.h>

Viewer::Viewer(QWidget* parent)
: QMainWindow(parent)
, mUi(new Ui::Viewer)
{
  mUi->setupUi(this);

  setupModules();

  connect(mUi->configure, &QPushButton::clicked, [this] {
    mPlaybackController.configure(this);
  });

  connect(mUi->play, &QToolButton::clicked, [this] {
    onStartCommand();
  });
  connect(mUi->stop, &QToolButton::clicked, [this] {
    onStopCommand();
  });
  connect(mUi->trigger, &QToolButton::clicked, [this] {
    onTriggerCommand();
  });
  mUi->stop->setEnabled(false);

  mImageStatsLabel = new QLabel(this);
  statusBar()->addPermanentWidget(mImageStatsLabel);

  mPlaybackController.setResultEvent([this](PlaybackController::TimedResult result) {
    updateImage(result);
  });

  mPlaybackController.setAbortEvent([this](std::string const& message) {
    onAborted(message);
  });
}

Viewer::~Viewer() = default;

void Viewer::setupModules()
{
  auto const& modules = mCameraModuleFactory.getAvailableModules();

  QString defaultModule = (!modules.empty()) ? modules.front().c_str() : "";

  QSettings settings;
  auto module = settings.value("camera/module", defaultModule).toString().toStdString();

  for (auto const& moduleName : modules) {
    mUi->module->addItem(moduleName.c_str(), QString(moduleName.c_str()));

    if (moduleName == module) {
      mUi->module->setCurrentIndex(mUi->module->count() - 1);
    }
  }
  mPlaybackController.change(mCameraModuleFactory.createModule(module));

  auto currentIndexChanged = static_cast<void (QComboBox::*)(QString const&)>(&QComboBox::currentIndexChanged);
  connect(mUi->module, currentIndexChanged, [this](QString module) {
    QSettings settings;
    settings.setValue("camera/module", module);
    mPlaybackController.change(mCameraModuleFactory.createModule(module.toStdString()));
    mUi->display->setImage({});
  });
}

void Viewer::updateImage(PlaybackController::TimedResult result)
{
  auto fps = result.frameTime.count() > 0.0 ? 1.0 / result.frameTime.count() : 0.0;
  auto ms = result.frameTime.count() * 1000.0;
  mUi->display->setImage(result.image);
  mImageStatsLabel->setText(QString("Frametime: %1 ms, FPS: %2, Min: %3, Max %4")
                                .arg(ms, 4, 'f', 1)
                                .arg(fps, 4, 'f', 1)
                                .arg(result.min)
                                .arg(result.max));
}

void Viewer::onStartCommand()
{
  try {
    mPlaybackController.start();
    mUi->configure->setEnabled(false);
    mUi->play->setEnabled(false);
    mUi->stop->setEnabled(true);
  } catch (std::exception const& e) {
    QMessageBox::critical(this, "Error starting camera", e.what());
  }
}

void Viewer::onStopCommand()
{
  try {
    mPlaybackController.stop();
    mUi->configure->setEnabled(true);
    mUi->play->setEnabled(true);
    mUi->stop->setEnabled(false);
  } catch (std::exception const& e) {
    QMessageBox::critical(this, "Error stopping camera", e.what());
  }
}

void Viewer::onTriggerCommand()
{
  try {
    mPlaybackController.trigger();
  } catch (std::exception const& e) {
    QMessageBox::critical(this, "Error stopping camera", e.what());
  }
}

void Viewer::onAborted(std::string const& message)
{
  QMessageBox::critical(this, "Acquisition aborted", message.c_str());
}
