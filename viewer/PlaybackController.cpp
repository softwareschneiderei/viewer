
#include "PlaybackController.h"
#include <QWidget>

PlaybackController::PlaybackController()
{

}

PlaybackController::~PlaybackController()
{

}

void PlaybackController::setCallback(AbstractImagePoller::ResultEvent event)
{
  stop();
  mEvent = std::move(event);
  start();
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
