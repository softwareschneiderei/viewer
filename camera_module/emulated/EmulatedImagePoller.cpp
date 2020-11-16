#include "EmulatedImagePoller.h"
#include <QColor>

namespace
{
// Hardcoded for now
auto const width=600;
auto const height=600;
auto const blockSize = 120;
}

void EmulatedImagePoller::startAcquisition()
{

}

void EmulatedImagePoller::poll()
{
  std::this_thread::sleep_for(std::chrono::milliseconds(33));
  QImage targetImage(width, height, QImage::Format_RGB32);

  QColor light(150, 150, 150);
  QColor dark(64, 64, 64);

  std::uniform_int_distribution<int> green(0, 60);

  for (int y=0; y<height; ++y)
  {
    int red = y*60/height;
    int by = (y/blockSize)&1;
    for (int x=0; x<width; ++x)
    {
      int blue = x*60/width;
      int bx = (x/blockSize)&1;
      auto color = (bx^by) ? light : dark;
      color.setRed(color.red() + red);
      color.setBlue(color.blue() + blue);
      color.setGreen(color.green() + green(mRng));

      targetImage.setPixel(x, y, color.rgb());
    }
  }
  //targetImage.fill(0xFFFFFFFF);

  dispatch(targetImage, 0, 0);
}

void EmulatedImagePoller::stopAcquisition()
{

}

QWidget* EmulatedImagePoller::configure(QWidget* parent)
{
  return nullptr;
}

void EmulatedImagePoller::trigger()
{
  // sic! do nothing for now
}
