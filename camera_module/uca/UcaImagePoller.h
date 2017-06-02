#pragma once

#include "ThreadedImagePoller.h"

#include <uca/uca-camera.h>

class UcaImagePoller : public ThreadedImagePoller
{
public:
  UcaImagePoller(std::string const& name);
  ~UcaImagePoller();

  void startAcquisition() override;

  void poll(bool toneMapping) override;

  void stopAcquisition() override;

private:
  UcaCamera* mCamera;
  guint mWidth;
  guint mHeight;
  guint mBits;
  std::vector<std::uint8_t> mBuffer;

};

