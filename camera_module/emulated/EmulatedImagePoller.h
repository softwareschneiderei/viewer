#pragma once
#include "ThreadedImagePoller.h"
#include <thread>
#include <atomic>
#include <random>

class EmulatedImagePoller : public ThreadedImagePoller
{
public:
  void startAcquisition() override;
  void poll() override;
  void stopAcquisition() override;
  void trigger() override;

  QWidget* configure(QWidget* parent) override;
private:
  std::default_random_engine mRng;
};
