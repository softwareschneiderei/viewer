#pragma once
#include "ThreadedImagePoller.h"
#include <thread>
#include <atomic>

class EmulatedImagePoller : public ThreadedImagePoller
{
  void startAcquisition() override;

  void poll(bool autoLeveling) override;

  void stopAcquisition() override;

public:
private:
};
