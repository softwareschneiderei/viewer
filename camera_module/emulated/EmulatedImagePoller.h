#pragma once
#include "ThreadedImagePoller.h"
#include <thread>
#include <atomic>

class EmulatedImagePoller : public ThreadedImagePoller
{
  void startAcquisition() override;

  void poll() override;

  void stopAcquisition() override;

  QWidget* configure(QWidget* parent) override;
public:
private:
};
