
#include "ThreadedImagePoller.h"
#include <iostream>
#include <vector>
#include <numeric>
#include <QImage>

namespace
{
void logError()
{
  try
  {
    throw;
  }
  catch (std::exception const &error)
  {
    std::cerr << "Error: " << error.what() << std::endl;
  }
  catch (...)
  {
    std::cerr << "Unknown error" << std::endl;
  }
}
}
ThreadedImagePoller::ThreadedImagePoller()
{
}

void ThreadedImagePoller::start(ResultEvent resultEvent, AbortEvent abortEvent)
{
  if (mKeepRunning)
  {
    return;
  }

  mResultEvent = std::move(resultEvent);
  mAbortEvent = std::move(abortEvent);
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
  try
  {
    startAcquisition();
  }
  catch(std::exception const& e)
  {
    logError();
    abort(e.what());
    return;
  }
  catch(...)
  {
    logError();
    mKeepRunning = false;
    return;
  }

  while (mKeepRunning)
  {
    try
    {
      poll();
      continue;
    }
    catch (...)
    {
      logError();
    }

    // Sleep a while after an error
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }

  try
  {
    stopAcquisition();
  }
  catch(...)
  {
    logError();
  }
}

void ThreadedImagePoller::dispatch(QImage image, uint16_t min, uint16_t max)
{
  mResultEvent({image, min, max});
}

void ThreadedImagePoller::dispatch(AbstractImagePoller::Result result)
{
  mResultEvent(result);

}

void ThreadedImagePoller::abort(std::string const &message)
{
  mKeepRunning = false;
  mAbortEvent(message);
}
