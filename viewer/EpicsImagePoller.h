#pragma once
#include <thread>
#include <functional>
#include <atomic>
#include <QImage>
#include "AbstractImagePoller.h"

class EpicsImagePoller : public AbstractImagePoller

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

    EpicsImagePoller(std::string prefix, int serialNumber);

    void start(std::function<void(Result image)> event);
    void stop();

    void setTonemapping(bool rhs);
private:
    void run();

    void startAcquisition();
    void poll();
    void stopAcquisition();

    void dispatch(QImage image, std::uint16_t min, std::uint16_t max);

    std::function<void(Result)> mEvent;
    std::thread mThread;
    std::atomic_bool mKeepRunning{false};
    std::atomic_bool mTonemapping{true};

    std::string mPrefix;
    int mSerialNumber;
    std::uint64_t mImageIndex=0;
    std::uint64_t mErrorIndex=0;
};
