#pragma once

#include "AbstractImagePoller.h"
#include <atomic>
#include <thread>

class ThreadedImagePoller : public AbstractImagePoller
{
public:
  ThreadedImagePoller();

  void start(ResultEvent event) override;
  void stop() override;
  void setAutoLeveling(bool rhs) override;

  virtual void startAcquisition() = 0;
  virtual void poll(bool autoLeveling) = 0;
  virtual void stopAcquisition() = 0;

  void dispatch(Result result);
  void dispatch(QImage image, std::uint16_t min, std::uint16_t max);
private:
  void run();
  
  ResultEvent mEvent;
  std::thread mThread;
  std::atomic_bool mKeepRunning{false};
  std::atomic_bool mAutoLeveling{true};
};

