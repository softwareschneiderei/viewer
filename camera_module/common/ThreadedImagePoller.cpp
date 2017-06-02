
#include "ThreadedImagePoller.h"
#include <iostream>
#include <vector>
#include <numeric>
#include <QImage>

ThreadedImagePoller::ThreadedImagePoller()
{
}

void ThreadedImagePoller::start(ResultEvent event)
{
  if (mKeepRunning)
  {
    return;
  }

  mEvent = event;
  mKeepRunning = true;
  mThread = std::thread([this]()
                        {
                          run();
                        });
}

void ThreadedImagePoller::stop()
{
  mKeepRunning = false;
  mThread.join();
}

void ThreadedImagePoller::run()
{
  startAcquisition();

  while (mKeepRunning)
  {
    try
    {
      poll(mAutoLeveling);
      continue;
    }
    catch (std::exception const &error)
    {
      std::cerr << "Error: " << error.what() << std::endl;
    }
    catch (...)
    {
      std::cerr << "Unknown error" << std::endl;
    }

    // Sleep a while after an error
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }

  stopAcquisition();
}

void ThreadedImagePoller::setAutoLeveling(bool rhs)
{
  mAutoLeveling = rhs;
}

void ThreadedImagePoller::dispatch(QImage image, uint16_t min, uint16_t max)
{
  mEvent({image, min, max});
}

void ThreadedImagePoller::dispatch(AbstractImagePoller::Result result)
{
  mEvent(result);

}
