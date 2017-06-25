#include "EmulatedImagePoller.h"

void EmulatedImagePoller::startAcquisition()
{

}

void EmulatedImagePoller::poll()
{
  std::this_thread::sleep_for(std::chrono::milliseconds(33));
  QImage targetImage(600, 600, QImage::Format_RGB32);
  targetImage.fill(0xFFFFFFFF);

  dispatch(targetImage, 0, 0);
}

void EmulatedImagePoller::stopAcquisition()
{

}

QWidget* EmulatedImagePoller::configure(QWidget* parent)
{
  return nullptr;
}
