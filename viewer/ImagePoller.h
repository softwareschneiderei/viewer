#pragma once
#include <thread>
#include <functional>
#include <atomic>
#include <QImage>

class ImagePoller
{
public:
    ImagePoller();

    void start(std::function<void(QImage image)> event);
    void stop();
private:
    void run();
    void poll();

    std::function<void(QImage)> mEvent;
    std::thread mThread;
    std::atomic_bool mKeepRunning{false};
};
