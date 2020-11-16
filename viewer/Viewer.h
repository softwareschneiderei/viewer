#pragma once

#include <QMainWindow>
#include <memory>
#include "AbstractImagePoller.h"
#include "CameraModuleFactory.h"
#include "PlaybackController.h"
#include <QLabel>

namespace Ui
{
  class Viewer;
}


class Viewer : public QMainWindow
{
Q_OBJECT

public:
  explicit Viewer(QWidget* parent = 0);

  ~Viewer();

private:
  void updateImage(PlaybackController::TimedResult result);
  void setupModules();
  void onStartCommand();
  void onStopCommand();
  void onTriggerCommand();
  void onAborted(std::string const& message);

  std::unique_ptr<Ui::Viewer> mUi;
  PlaybackController mPlaybackController;
  CameraModuleFactory mCameraModuleFactory;

  QLabel* mImageStatsLabel;

};

