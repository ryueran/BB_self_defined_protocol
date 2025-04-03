#include "Handler.hh"

void Handler::enable_close()
{
    event_type_ = EventType::Close;
}

void Handler::enable_read()
{
    event_type_ = EventType::Read;
}

void Handler::enable_write()
{
    event_type_ = EventType::Write;
}

void Handler::setCloseCallback(CallBack cl_callback)
{
    close_callback_ = cl_callback;
}

void Handler::setReadCallback(CallBack re_callback)
{
    read_callback_ = re_callback;
}

void Handler::setWriteCallback(CallBack wr_callback)
{
    write_callback_ = wr_callback;
}

int Handler::get_handler_fd()
{
    return handler_fd_;
}

void Handler::set_in_Epoll()
{
    isInEpoll_ = true;
}

void Handler::set_out_Epoll()
{
    isInEpoll_ = false;
}

EventType Handler::get_handler_event()
{
    return event_type_;
}

void Handler::handle_event()
{
    if(event_type_ == EventType::Write)
    {
        write_callback_(handler_fd_);
    }

    if(event_type_ == EventType::Read)
    {
        read_callback_(handler_fd_);
    }

    if(event_type_ == EventType::Close)
    {
        close_callback_(handler_fd_);
    }
}

bool Handler::is_in_Epoll()
{
    return isInEpoll_;
}
