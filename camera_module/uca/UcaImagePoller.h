#pragma once

#include "ThreadedImagePoller.h"
#include "UcaState.h"


class UcaImagePoller : public ThreadedImagePoller
{
public:
  explicit UcaImagePoller(std::string const& name);
  ~UcaImagePoller() override;

  void startAcquisition() override;

  void poll() override;

  void stopAcquisition() override;
  void trigger() override;

  QWidget* configure(QWidget* parent) override;
private:
  std::shared_ptr<UcaState> mState;
  guint mWidth = 0;
  guint mHeight = 0;
  guint mBits = 0;
  std::vector<std::uint8_t> mBuffer;

};

