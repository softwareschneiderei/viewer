#pragma once

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
  ~AbstractImagePoller();

  virtual void start(std::function<void(Result image)> event);
  virtual void stop();

  virtual void setTonemapping(bool rhs);
};
