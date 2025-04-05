#ifndef SERVER_HH
#define SERVER_HH

#include <iostream>
#include <map>
#include "Acceptor.hh"
#include "Handler.hh"
#include "Reactor.hh"

class Server
{
public:
    Server()
    {
        std::cout << "Server constructed!" << std::endl;
    }

    ~Server()
    {
        std::cout << "Server destructed!" << std::endl;
        // delete[] message_buffer;
    }
    void start_server(); // Acceptor.bind, listen, accept, handler->register(std::bind(read_client, this, _1))
    void accept(int server_fd);
    void read_client(int fd); // handler->register(std::bind(write_client, this, _1))
    void write_client(int fd);
private:
    Acceptor acceptor_;
    Reactor reactor_;
    std::vector<uint8_t> message_buffer = std::vector<uint8_t>(1024);
    std::map<int, std::shared_ptr<Handler>> handlers_;
    std::map<uint32_t, uint32_t> session_; // <fd, cliend_id>
    int fd_;
    uint8_t client_id_counter_ = 0;
};

#endif