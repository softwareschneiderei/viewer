#pragma once

#include "ThreadedImagePoller.h"

#include <uca/uca-camera.h>

class UcaImagePoller : public ThreadedImagePoller
{
public:
  UcaImagePoller(std::string const& name);

  void startAcquisition() override;

  void poll(bool toneMapping) override;

  void stopAcquisition() override;

private:
  UcaCamera* mCamera;
};

