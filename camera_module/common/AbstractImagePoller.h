#pragma once
#include <QImage>
#include <functional>

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

  using ResultEvent = std::function<void(Result)>;

  AbstractImagePoller();
  virtual ~AbstractImagePoller();

  virtual void start(ResultEvent event)=0;
  virtual void stop()=0;

  virtual void setTonemapping(bool rhs)=0;
};
