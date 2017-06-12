#pragma once

#include <QMainWindow>
#include <memory>
#include "EpicsImagePoller.h"
#include "CameraModuleFactory.h"
#include <QLabel>

namespace Ui
{
  class Viewer;
}


class Viewer : public QMainWindow
{
Q_OBJECT

public:
  explicit Viewer(QString device, int serial, QWidget* parent = 0);

  ~Viewer();

private:
  void updateImage(EpicsImagePoller::Result result);
  void changePoller(std::shared_ptr<AbstractImagePoller> poller);
  std::unique_ptr<Ui::Viewer> mUi;
  std::shared_ptr<AbstractImagePoller> mPoller;
  CameraModuleFactory mCameraModuleFactory;

  QLabel* mImageStatsLabel;
};

