#pragma once
#include "Channel.h"

class Device
{
public:
    Device(std::string const& prefix);

    template <class T>
    void put(std::string const& variable, T value)
    {
        Channel access(mPrefix+variable);
        Channel::wait(1.0);
        access.put(value);
        Channel::wait(1.0);
    }

private:
    std::string mPrefix;
};
