#include <cadef.h>
#include <db_access.h>
#include <iostream>


int main(int argc, char** argv)
{
    chid channel;
    capri priority = 0;
    int result = ca_create_channel("HELGETEST1-CCDCAM:WIDTH", nullptr, nullptr, priority, &channel);
    if (result != ECA_NORMAL)
    {
        return -1;
    }

    result = ca_pend_io(1.0);
    if (result != ECA_NORMAL)
        return -1;

    auto state = ca_state(channel);
    if (state == cs_conn)
        std::cout << "Connected!" << std::endl;
    else
        std::cout << "State is " << state << std::endl;

    std::uint32_t width=0;
    ca_array_get(DBR_INT, 1, channel, &width);
    result = ca_pend_io(1.0);
    if (result != ECA_NORMAL)
        return -1;

    std::cout << "Width is " << width << std::endl;

    ca_clear_channel(channel);
    return 0;
}
