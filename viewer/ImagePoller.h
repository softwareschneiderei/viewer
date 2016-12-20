#pragma once
#include <thread>
#include <functional>
#include <atomic>
#include <QImage>

class ImagePoller
{
public:
    struct Result
    {
        QImage image;
        std::uint16_t min;
        std::uint16_t max;
    };

    ImagePoller(std::string prefix, int serialNumber);

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
};
