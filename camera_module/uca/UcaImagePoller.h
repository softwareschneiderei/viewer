#pragma once

#include "ThreadedImagePoller.h"

#include <uca/uca-camera.h>
#include <uca/uca-plugin-manager.h>

class UcaImagePoller : public ThreadedImagePoller
{
public:
  UcaImagePoller(std::string const& name);
  ~UcaImagePoller();

  void startAcquisition() override;

  void poll(bool toneMapping) override;

  void stopAcquisition() override;

  QWidget* configure(QWidget* parent) override;
private:
  UcaPluginManager* mManager;
  UcaCamera* mCamera;
  guint mWidth;
  guint mHeight;
  guint mBits;
  std::vector<std::uint8_t> mBuffer;

};

