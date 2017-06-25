
#include "PlaybackController.h"
#include "FunctionEvent.h"
#include <QWidget>

PlaybackController::PlaybackController()
{

}

PlaybackController::~PlaybackController()
{
  stop();
}

void PlaybackController::setResultEvent(TimedResultEvent event)
{
  stop();
  mResultEvent = [=](AbstractImagePoller::Result result)
  {
    FunctionEvent::post([=]
                        {
                          auto now = Clock::now();
                          auto frameTime = std::chrono::duration_cast<Duration>(now - mStart);
                          event(TimedResult(result, frameTime));
                          mStart = now;
                        });
  };
}

void PlaybackController::change(std::shared_ptr<AbstractImagePoller> poller)
{
  stop();
  mPoller = std::move(poller);
}

void PlaybackController::start()
{
  if (mStarted || !mPoller || !mResultEvent || !mAbortEvent)
    return;

  mStart = Clock::now();
  mPoller->start(mResultEvent, mAbortEvent);
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

void PlaybackController::setAbortEvent(PlaybackController::AbortEvent event)
{
  mAbortEvent = [this, event](std::string const& message)
  {
    FunctionEvent::post([=]
                        {
                          mStarted = false;
                          event(message);
                        });
  };
}
