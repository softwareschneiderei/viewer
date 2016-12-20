#include "Channel.h"
#include <stdexcept>

namespace
{

void checkReturn(int returnValue)
{
    if (returnValue != ECA_NORMAL)
    {
        throw std::runtime_error(std::string("Abnormal return value: ") + ca_message(returnValue));
    }
}

}

Channel::Channel(const std::string& name, capri priority)
{
    checkReturn(ca_create_channel(name.c_str(), nullptr, nullptr, priority, &mHandle));

}

Channel::~Channel()
{
    ca_clear_channel(mHandle);
}

channel_state Channel::state() const
{
    return ca_state(mHandle);
}

void Channel::array_get(chtype type, unsigned long count, void *pValue)
{
    checkReturn(ca_array_get(type, count, mHandle, pValue));
}

void Channel::wait(double time)
{
    checkReturn(ca_pend_io(time));
}
