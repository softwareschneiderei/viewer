#pragma once
#include <cadef.h>
#include <string>

class Channel
{
public:
    Channel(std::string const& name, capri priority=0);
    Channel(Channel const&) = delete;
    ~Channel();
    Channel& operator=(Channel const&) = delete;

    channel_state state() const;



    void array_get(chtype type, unsigned long count, void* pValue);
    void put(chtype type, void* value);
    void get(chtype type, void* value);

    template <class T>
    T get()
    {
        T value;
        get(value);
        return value;
    }

    void get(int& value);
    void put(int value);

    bool is_connected() const;

    static void wait(double time);

private:
    chid mHandle;
};
