#ifndef REACTOR_HH
#define REACTOR_HH

#include <iostream>
#include <vector>
#include <memory>
#include <unistd.h>
#include "Handler.hh"
#include "Epoller.hh"

class Reactor
{
public:
    Reactor()
    {
        std::cout << "Reactor constructed!" << std::endl;
    }

    ~Reactor()
    {
        std::cout << "Reactor destructed!" << std::endl;
    }

    void register_handler(Handler* ptr_handler);
    void remove_handler(Handler* ptr_handler);
    void loop();
private:
    std::unique_ptr<Epoller> ptr_epoller = std::unique_ptr<Epoller>(new Epoller());
    int epoll_fd_;
};

#endif