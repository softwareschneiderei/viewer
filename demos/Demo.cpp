#include <cadef.h>
#include <db_access.h>
#include <iostream>
#include "Device.h"
#include <vector>
#include <numeric>
#include <thread>


void start(std::string const& prefix)
{
    Device device(prefix);
    device.put("SERIALNR", 21721600);
    device.put("INIT", 1);

    std::this_thread::sleep_for(std::chrono::seconds(2));
    device.put("RECMODE", 1);
    device.put("CAMERA", 1);

    std::this_thread::sleep_for(std::chrono::seconds(10));

    device.put("CAMERA", 0);
    device.put("INIT", 0);
}

int run(int argc, char** argv)
{
    // About enough memory for a 1280x1024
    setenv("EPICS_CA_MAX_ARRAY_BYTES", "3000000", true);


    std::string prefix = "HELGETEST1-CCDCAM:";

    start(prefix);

    /*Channel width_channel(prefix+"WIDTH");
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

    if (width <= 0 && height <= 0)
    {
        return -1;
    }

    std::vector<std::uint16_t> image(width*height);
    image_channel.array_get(DBR_INT, image.size(), image.data());

    Channel::wait(1.0);

    auto mean = std::accumulate(image.begin(), image.end(), 0) / static_cast<double>(image.size());
    std::cout << "Mean is " << mean << std::endl;*/

    return 0;
}

int main(int argc, char** argv)
{
    try
    {
        return run(argc, argv);
    }
    catch(std::exception const& error)
    {
        std::cerr << "Error: " << error.what() << std::endl;
    }
    catch(...)
    {
        std::cerr << "Error" << std::endl;
        return -1;
    }
}
