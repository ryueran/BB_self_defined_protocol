#include "Reactor.hh"

void Reactor::register_handler(Handler* ptr_handler)
{
    ptr_epoller->update(ptr_handler);
}

void Reactor::remove_handler(Handler* ptr_handler)
{
    ptr_epoller->remove(ptr_handler);
}

void Reactor::loop()
{
    while(1)
    {
        std::vector<Handler *> active_channel_list = ptr_epoller->poll();
        for(auto a_ch : active_channel_list)
        {
            a_ch->handle_event();
        }
    }
}