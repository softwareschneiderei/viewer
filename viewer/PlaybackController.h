#pragma once

#include "AbstractImagePoller.h"
#include <memory>
#include <chrono>

class PlaybackController
{
public:
  using Clock = std::chrono::high_resolution_clock;
  using Duration = std::chrono::duration<double>;

  PlaybackController();
  ~PlaybackController();

  struct TimedResult :
    public AbstractImagePoller::Result
  {
  public:
    TimedResult(AbstractImagePoller::Result base, Duration frameTime)
    : AbstractImagePoller::Result(base), frameTime(frameTime)
    {
    }

    Duration frameTime;
  };

  using TimedResultEvent = std::function<void(TimedResult)>;

  void setCallback(TimedResultEvent event);
  void change(std::shared_ptr<AbstractImagePoller> poller);
  void start();
  void stop();
  void configure(QWidget* parent);

private:
  std::shared_ptr<AbstractImagePoller> mPoller;
  AbstractImagePoller::ResultEvent mEvent;
  bool mStarted = false;
  Clock::time_point mStart;
};


