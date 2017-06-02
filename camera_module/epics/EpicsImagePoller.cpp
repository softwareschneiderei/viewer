#include "EpicsImagePoller.h"
#include "Channel.h"
#include "Device.h"
#include <iostream>
#include <vector>
#include <numeric>
#include <QImage>

EpicsImagePoller::EpicsImagePoller(std::string prefix, int serialNumber)
  : mPrefix(prefix), mSerialNumber(serialNumber)
{
}

void EpicsImagePoller::startAcquisition()
{
  Device device(mPrefix);

  device.put("SERIALNR", mSerialNumber);
  device.put("INIT", 1);
  std::this_thread::sleep_for(std::chrono::seconds(2));
  device.put("RECMODE", 1);
  device.put("EXPOSURE", 100);
  device.put("CAMERA", 1);
}

void EpicsImagePoller::poll(bool toneMapping)
{
  std::this_thread::sleep_for(std::chrono::milliseconds(66));

  Channel width_channel(mPrefix + "WIDTH");
  Channel height_channel(mPrefix + "HEIGHT");
  Channel image_channel(mPrefix + "FPICTURE");
  //Channel transfer_channel(prefix+"TRANSFER");

  Channel::wait(1.0);

  if (width_channel.state() != cs_conn ||
      height_channel.state() != cs_conn ||
      image_channel.state() != cs_conn/* ||
        !transfer_channel.is_connected()*/)
  {
    return;
  }

  std::int16_t width = 0;
  std::int16_t height = 0;
  width_channel.array_get(DBR_INT, 1, &width);
  height_channel.array_get(DBR_INT, 1, &height);
  //transfer_channel.put(1);
  Channel::wait(1.0);

  if (width <= 0 || height <= 0)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    return;
  }

  std::vector<std::uint16_t> image(width * height);
  image_channel.array_get(DBR_INT, image.size(), image.data());

  Channel::wait(1.0);

  auto minMax = std::minmax_element(image.begin(), image.end());

  if (toneMapping)
  {

    QImage targetImage(width, height, QImage::Format_RGB32);

    for (int y = 0; y < height; ++y)
    {
      auto target = reinterpret_cast<QRgb*>(targetImage.scanLine(y));
      auto source = image.begin() + y * width;
      for (int x = 0; x < width; ++x)
      {
        int gray = (source[x] - *minMax.first) * 255 / std::max(*minMax.second - *minMax.first, 1);
        target[x] = qRgb(gray, gray, gray);
      }
    }

    dispatch(targetImage, *minMax.first, *minMax.second);
  }
  else
  {
    QImage targetImage(width, height, QImage::Format_RGB32);

    for (int y = 0; y < height; ++y)
    {
      auto target = reinterpret_cast<QRgb*>(targetImage.scanLine(y));
      auto source = image.begin() + y * width;
      for (int x = 0; x < width; ++x)
      {
        int gray = source[x] >> 8;
        target[x] = qRgb(gray, gray, gray);
      }
    }

    dispatch(targetImage, *minMax.first, *minMax.second);
  }
}

void EpicsImagePoller::stopAcquisition()
{
  Device device(mPrefix);
  device.put("CAMERA", 0);
  device.put("INIT", 0);
}

