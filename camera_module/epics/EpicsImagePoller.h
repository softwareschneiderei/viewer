#pragma once

#include <thread>
#include <functional>
#include <atomic>
#include <QImage>
#include "AbstractImagePoller.h"

class EpicsImagePoller : public AbstractImagePoller
{
public:
  EpicsImagePoller(std::string prefix, int serialNumber);

  void start(ResultEvent event) override;

  void stop() override;

  void setTonemapping(bool rhs) override;

private:
  void run();

  void startAcquisition();

  void poll();

  void stopAcquisition();

  void dispatch(QImage image, std::uint16_t min, std::uint16_t max);

  ResultEvent mEvent;
  std::thread mThread;
  std::atomic_bool mKeepRunning{false};
  std::atomic_bool mTonemapping{true};

  std::string mPrefix;
  int mSerialNumber;
  std::uint64_t mImageIndex = 0;
  std::uint64_t mErrorIndex = 0;
};
