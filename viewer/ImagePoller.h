#pragma once
#include <thread>
#include <functional>
#include <atomic>

class ImagePoller
{
public:
    ImagePoller(std::function<void()> event);

    void start();
    void stop();
private:
    void run();
    void poll();

    std::function<void()> mEvent;
    std::thread mThread;
    std::atomic_bool mKeepRunning{false};
};
