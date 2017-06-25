
#include "PlaybackController.h"
#include <QWidget>

PlaybackController::PlaybackController()
{

}

PlaybackController::~PlaybackController()
{
  stop();
}

void PlaybackController::setCallback(TimedResultEvent event)
{
  stop();
  mEvent = [=](AbstractImagePoller::Result result)
  {
    auto now = Clock::now();
    auto frameTime = std::chrono::duration_cast<Duration>(now - mStart);
    event(TimedResult(result, frameTime));
    mStart = now;
  };
}

void PlaybackController::change(std::shared_ptr<AbstractImagePoller> poller)
{
  stop();
  mPoller = std::move(poller);
}

void PlaybackController::start()
{
  if (mStarted || !mPoller || !mEvent)
    return;

  mStart = Clock::now();
  mPoller->start(mEvent);
  mStarted = true;
}

void PlaybackController::stop()
{
  if (!mStarted || !mPoller)
    return;

  mPoller->stop();
  mStarted = false;
}

void PlaybackController::configure(QWidget* parent)
{
  if (mStarted || !mPoller)
    return;

  auto configurationWidget = mPoller->configure(parent);
  if (!configurationWidget)
  {
    return;
  }
  configurationWidget->show();
}
