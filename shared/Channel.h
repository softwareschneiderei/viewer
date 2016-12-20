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

    static void wait(double time);

private:
    chid mHandle;
};
