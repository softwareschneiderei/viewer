#pragma once
#include <QImage>

class AbstractImagePoller
{
public:
  struct Result
  {
    QImage image;
    std::uint16_t min;
    std::uint16_t max;
    std::uint64_t imageIndex;
    std::uint16_t errorIndex;
  };

  AbstractImagePoller();
  virtual ~AbstractImagePoller();

  virtual void start(std::function<void(Result image)> event)=0;
  virtual void stop()=0;

  virtual void setTonemapping(bool rhs)=0;
};
