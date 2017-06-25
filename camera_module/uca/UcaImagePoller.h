#pragma once

#include "ThreadedImagePoller.h"
#include "UcaState.h"


class UcaImagePoller : public ThreadedImagePoller
{
public:
  UcaImagePoller(std::string const& name);
  ~UcaImagePoller();

  void startAcquisition() override;

  void poll() override;

  void stopAcquisition() override;

  QWidget* configure(QWidget* parent) override;
private:
  std::shared_ptr<UcaState> mState;
  guint mWidth;
  guint mHeight;
  guint mBits;
  std::vector<std::uint8_t> mBuffer;

};

