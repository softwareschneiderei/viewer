#include "ImagePoller.h"
#include "Channel.h"
#include "Device.h"
#include <iostream>
#include <vector>
#include <numeric>
#include <QImage>

ImagePoller::ImagePoller(std::string prefix, int serialNumber)
: mPrefix(prefix), mSerialNumber(serialNumber)
{
}

void ImagePoller::start(std::function<void (Result)> event)
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
    startAcquisition();

    while(mKeepRunning)
    {
        try
        {
            poll();
            continue;
        }
        catch(std::exception const& error)
        {
            std::cerr << "Error: " << error.what() << std::endl;
        }
        catch(...)
        {
            std::cerr << "Unknown error" << std::endl;
        }

        // Sleep a while after an error
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    stopAcquisition();
}

void ImagePoller::setTonemapping(bool rhs)
{
    mTonemapping = rhs;
}

void ImagePoller::startAcquisition()
{
    Device device(mPrefix);

    device.put("SERIALNR", mSerialNumber);
    device.put("INIT", 1);
    std::this_thread::sleep_for(std::chrono::seconds(2));
    device.put("RECMODE", 1);
    device.put("EXPOSURE", 100);
    device.put("CAMERA", 1);
}

void ImagePoller::poll()
{

    Channel width_channel(mPrefix+"WIDTH");
    Channel height_channel(mPrefix+"HEIGHT");
    Channel image_channel(mPrefix+"FPICTURE");
    //Channel transfer_channel(prefix+"TRANSFER");

    Channel::wait(1.0);

    if (width_channel.state() != cs_conn ||
        height_channel.state() != cs_conn ||
        image_channel.state() != cs_conn/* ||
        !transfer_channel.is_connected()*/)
        return;

    std::int16_t width=0;
    std::int16_t height=0;
    width_channel.array_get(DBR_INT, 1, &width);
    height_channel.array_get(DBR_INT, 1, &height);
    //transfer_channel.put(1);
    Channel::wait(1.0);

    if (width <= 0 || height <= 0)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        return;
    }

    std::vector<std::uint16_t> image(width*height);
    image_channel.array_get(DBR_INT, image.size(), image.data());

    Channel::wait(1.0);

    auto minMax = std::minmax_element(image.begin(), image.end());

    if (mTonemapping)
    {

        QImage targetImage(width, height, QImage::Format_Grayscale8);

        for (int y=0; y<height; ++y)
        {
            auto target = targetImage.scanLine(y);
            auto source = image.begin() + y*width;
            for (int x =0; x<width; ++x)
                target[x] = (source[x]-*minMax.first)*255 / std::max(*minMax.second-*minMax.first, 1);
        }

        dispatch(targetImage, *minMax.first, *minMax.second);
    }
    else
    {
        QImage targetImage(width, height, QImage::Format_Grayscale8);

        for (int y=0; y<height; ++y)
        {
            auto target = targetImage.scanLine(y);
            auto source = image.begin() + y*width;
            for (int x =0; x<width; ++x)
                target[x] = source[x] >> 8;
        }

        dispatch(targetImage, *minMax.first, *minMax.second);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(66));
}

void ImagePoller::stopAcquisition()
{
    Device device(mPrefix);
    device.put("CAMERA", 0);
    device.put("INIT", 0);
}

void ImagePoller::dispatch(QImage image, uint16_t min, uint16_t max)
{
    QImage scaledImage = image.scaledToWidth(600);
    mEvent({scaledImage, min, max});
}
