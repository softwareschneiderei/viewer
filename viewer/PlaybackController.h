#pragma once

#include "AbstractImagePoller.h"
#include <memory>

class PlaybackController
{
public:
  PlaybackController();
  ~PlaybackController();

  void setCallback(AbstractImagePoller::ResultEvent event);
  void change(std::shared_ptr<AbstractImagePoller> poller);
  void start();
  void stop();
  void configure(QWidget* parent);

private:
  std::shared_ptr<AbstractImagePoller> mPoller;
  AbstractImagePoller::ResultEvent mEvent;
  bool mStarted = false;
};


