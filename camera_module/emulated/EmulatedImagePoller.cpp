#include "EmulatedImagePoller.h"

void EmulatedImagePoller::startAcquisition()
{

}

void EmulatedImagePoller::poll(bool toneMapping)
{
  std::this_thread::sleep_for(std::chrono::milliseconds(33));
  QImage targetImage(600, 600, QImage::Format_RGB32);

  dispatch(targetImage, 0, 0);
}

void EmulatedImagePoller::stopAcquisition()
{

}
