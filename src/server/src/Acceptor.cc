#include "Acceptor.hh"

Acceptor::Acceptor()
{
    std::cout << "Acceptor constructed!" << std::endl;
    std::cout << "Construct sockt file!" << std::endl;

    if ((server_fd_ = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "Configure server address!" << std::endl;

    server_addr_.sin_family = AF_INET;
    server_addr_.sin_addr.s_addr = INADDR_ANY;
    server_addr_.sin_port = htons(PORT);
}

Acceptor::~Acceptor()
{
    std::cout << "Acceptor destructed!" << std::endl;
}

void Acceptor::server_bind()
{
    // Bind the socket
    if (bind(server_fd_, reinterpret_cast<sockaddr *>(&server_addr_), sizeof(server_addr_)) < 0) { // here is dangrous, has to be carefull
        perror("bind failed");
        close(server_fd_);
        exit(EXIT_FAILURE);
    }
}

void Acceptor::server_listen()
{
    // Start listening
    if (listen(server_fd_, 10) < 0) {
        perror("listen");
        close(server_fd_);
        exit(EXIT_FAILURE);
    }

    int flags = fcntl(server_fd_, F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl(F_GETFL)");
        return;
    }
    if (fcntl(server_fd_, F_SETFL, flags | O_NONBLOCK) == -1) {
        perror("fcntl(F_SETFL)");
        return;
    }
}

void Acceptor::server_accept(int server_fd) // tech debt
{
    socklen_t addrlen = sizeof(server_addr_);
    socket_fd_ = accept(server_fd, reinterpret_cast<sockaddr *>(&server_addr_), &addrlen); // here is dangerous, has to be carefull
    std::cout << "New Socket fd: " << socket_fd_ << std::endl; // The problem happens here, the new connection that calls the accept will overload the old acceüt of connection
    int flags = fcntl(socket_fd_, F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl(F_GETFL)");
        return;
    }
    if (fcntl(socket_fd_, F_SETFL, flags | O_NONBLOCK) == -1) {
        perror("fcntl(F_SETFL)");
        return;
    }
}

int Acceptor::get_socket_fd()
{
    return socket_fd_;
}

int Acceptor::get_server_fd()
{
    return server_fd_;
}
