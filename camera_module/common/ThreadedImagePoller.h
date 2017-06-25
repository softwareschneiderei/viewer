#pragma once

#include "AbstractImagePoller.h"
#include <atomic>
#include <thread>

class ThreadedImagePoller : public AbstractImagePoller
{
public:
  ThreadedImagePoller();

  void start(ResultEvent resultEvent, AbortEvent abortEvent) override;
  void stop() override;

  virtual void startAcquisition() = 0;
  virtual void poll() = 0;
  virtual void stopAcquisition() = 0;

  void dispatch(Result result);
  void dispatch(QImage image, std::uint16_t min, std::uint16_t max);
private:
  void run();
  void abort(std::string const& message);
  
  ResultEvent mResultEvent;
  AbortEvent mAbortEvent;
  std::thread mThread;
  std::atomic_bool mKeepRunning{false};
};

