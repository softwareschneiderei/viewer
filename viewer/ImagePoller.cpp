#include "ImagePoller.h"
#include "Channel.h"
#include <iostream>
#include <vector>
#include <numeric>

ImagePoller::ImagePoller(std::function<void()> event)
    : mEvent(event)
{
}

void ImagePoller::start()
{
    if (mKeepRunning)
        return;

    mKeepRunning = true;
    mThread = std::thread([this]()
    {
        run();
    });
}

void ImagePoller::stop()
{
    mKeepRunning = false;
    mThread.join();
}

void ImagePoller::run()
{
    while(mKeepRunning)
    {
        try
        {
            poll();
        }
        catch(std::exception const& error)
        {
            std::cerr << "Error: " << error.what() << std::endl;
        }
        catch(...)
        {
            std::cerr << "Unknown error" << std::endl;
        }
    }
}

void ImagePoller::poll()
{
    std::string prefix = "HELGETEST1-CCDCAM:";

    Channel width_channel(prefix+"WIDTH");
    Channel height_channel(prefix+"HEIGHT");
    Channel image_channel(prefix+"FPICTURE");

    Channel::wait(1.0);

    auto state = width_channel.state();
    if (state == cs_conn)
        std::cout << "Connected!" << std::endl;
    else
        std::cout << "State is " << state << std::endl;

    std::int16_t width=0;
    std::int16_t height=0;
    width_channel.array_get(DBR_INT, 1, &width);
    height_channel.array_get(DBR_INT, 1, &height);
    Channel::wait(1.0);
    std::cout << "Size is " << width << "x" << height << std::endl;

    if (width <= 0 || height <= 0)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        return;
    }

    std::vector<std::uint16_t> image(width*height);
    image_channel.array_get(DBR_INT, image.size(), image.data());

    Channel::wait(1.0);

    auto mean = std::accumulate(image.begin(), image.end(), 0) / static_cast<double>(image.size());
    std::cout << "Mean is " << mean << std::endl;
}
