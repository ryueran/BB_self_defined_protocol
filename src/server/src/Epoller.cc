#include "Epoller.hh"
#include "Handler.hh"

void Epoller::update(Handler* ptr_handler)
{
    epoll_event event;
    event.events = (ptr_handler->get_handler_event() == EventType::Read) ? EPOLLIN | EPOLLET : EPOLLOUT;

    event.data.ptr = ptr_handler;
    int ret;
    if(ptr_handler->is_in_Epoll())
    {
        ret = epoll_ctl(epfd_, EPOLL_CTL_MOD, ptr_handler->get_handler_fd(), &event);
    } else {
        ret = epoll_ctl(epfd_, EPOLL_CTL_ADD, ptr_handler->get_handler_fd(), &event);
        ptr_handler->set_in_Epoll();
    }
    if(ret < 0)
    {
        perror("epoll_ctl ADD failed!");
    } else {
        std::cout << "epoll event added! " << ptr_handler->get_handler_fd()  << " " << int(ptr_handler->get_handler_event()) << std::endl;
    }
}

void Epoller::remove(Handler* ptr_handler)
{
    if(epoll_ctl(epfd_, EPOLL_CTL_DEL, ptr_handler->get_handler_fd(), nullptr) < 0)
    {
        ptr_handler->set_out_Epoll();
        perror("epoll_ctl DEL failed!");
    } else {
        std::cout << "epoll event deleted!" << std::endl;
    }
}

std::vector<Handler *> Epoller::poll()
{
    std::vector<Handler *> handler_list = {};
    
    int n_event = epoll_wait(epfd_, epoll_events_.data(), epoll_events_.size(), -1); // epoll_event consumer
    if (n_event == -1) {
        perror("epoll_wait");
        return handler_list;
    }

    for (int i = 0; i < n_event; ++i) {
        auto& event = epoll_events_[i];
        // std::shared_ptr<Handler> handler_ptr(static_cast<Handler*>(event.data.ptr));
        Handler* handler_ptr = static_cast<Handler*>(event.data.ptr);
        if (handler_ptr == nullptr) {
            std::cout << "Error: handler_ptr is nullptr!" << std::endl;
            continue;
        }

        // if (event.events & EPOLLIN) {
        //     handler_ptr->enable_read();
        // }
        // if (event.events & EPOLLOUT) {
        //     handler_ptr->enable_write();
        // }

        handler_list.push_back(handler_ptr);
    }
    return handler_list;
}
