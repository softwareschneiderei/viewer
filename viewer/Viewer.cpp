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
  mPoller = std::make_shared<UcaImagePoller>("mock");

  mUi->setupUi(this);
  mImageStatsLabel = new QLabel(this);
  statusBar()->addPermanentWidget(mImageStatsLabel);

  connect(mUi->tonemapping, &QToolButton::clicked, [this](bool checked)
  {
    mPoller->setAutoLeveling(checked);
  });

  mPoller->start([this](EpicsImagePoller::Result result)
                 {
                   updateImage(result);
                 });
}

Viewer::~Viewer()
{
  mPoller->stop();
}

void Viewer::updateImage(EpicsImagePoller::Result result)
{
  ;
  mUi->display->setImage(result.image);
  mImageStatsLabel->setText(QString("Min: %1, Max %2").arg(result.min).arg(result.max));
}
