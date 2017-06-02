#pragma once

#include <thread>
#include <functional>
#include <atomic>
#include <QImage>
#include "ThreadedImagePoller.h"

class EpicsImagePoller : public ThreadedImagePoller
{
public:
  EpicsImagePoller(std::string prefix, int serialNumber);


  void startAcquisition() override;

  void poll(bool toneMapping) override;

  void stopAcquisition() override;

private:
  std::string mPrefix;
  int mSerialNumber;
  std::uint64_t mImageIndex = 0;
  std::uint64_t mErrorIndex = 0;
};
