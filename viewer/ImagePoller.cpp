#include "ImagePoller.h"
#include "Channel.h"
#include <iostream>
#include <vector>
#include <numeric>
#include <QImage>

ImagePoller::ImagePoller()
{
}

void ImagePoller::start(std::function<void (QImage)> event)
{
    if (mKeepRunning)
        return;

    mEvent = event;
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

    if (width_channel.state() != cs_conn ||
        height_channel.state() != cs_conn ||
        image_channel.state() != cs_conn)
        return;

    std::int16_t width=0;
    std::int16_t height=0;
    width_channel.array_get(DBR_INT, 1, &width);
    height_channel.array_get(DBR_INT, 1, &height);
    Channel::wait(1.0);

    if (width <= 0 || height <= 0)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        return;
    }

    std::vector<std::uint16_t> image(width*height);
    image_channel.array_get(DBR_INT, image.size(), image.data());

    Channel::wait(1.0);

    auto mean = std::accumulate(image.begin(), image.end(), 0) / static_cast<double>(image.size());

    auto minMax = std::minmax_element(image.begin(), image.end());

    QImage targetImage(width, height, QImage::Format_Grayscale8);

    for (int y=0; y<height; ++y)
    {
        auto target = targetImage.scanLine(y);
        auto source = image.begin() + y*width;
        for (int x =0; x<width; ++x)
            target[x] = (source[x]-*minMax.first)*255 / std::max(*minMax.second-*minMax.first, 1);
    }

    mEvent(targetImage);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
}
