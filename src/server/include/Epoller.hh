#ifndef EPOLLER_HH
#define EPOLLER_HH

#include <iostream>
#include <memory>
#include <vector>
#include <unistd.h>
#include <sys/epoll.h>

#define MAX_EVENTS 20

class Handler;
class Epoller
{
public:
    Epoller(): epoll_events_(32) {
        std::cout << "Epoller constructed!" << std::endl;
        epfd_ = epoll_create1(EPOLL_CLOEXEC);
    }

    ~Epoller()
    {
        std::cout << "Epoller destructed!" << std::endl;
        close(epfd_);
    }

    void update(Handler* ptr_handler);
    void remove(Handler* ptr_handler);
    std::vector<Handler *> poll();

private:
    int epfd_;
    std::vector<::epoll_event> epoll_events_; 
};

#endif