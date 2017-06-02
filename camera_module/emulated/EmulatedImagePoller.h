#pragma once
#include "AbstractImagePoller.h"

class EmulatedImagePoller : public AbstractImagePoller
{
public:
  void start(ResultEvent event) override;
  void stop() override;

  void setTonemapping(bool rhs) override;
private:
  ResultEvent mEvent;
};
