#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <errno.h>
#include <fcntl.h>

#define MAX_EVENTS 10
#define PORT 8080
#define BUFFER_SIZE 1024

// Set a file descriptor to non-blocking mode
int set_nonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl(F_GETFL)");
        return -1;
    }
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
        perror("fcntl(F_SETFL)");
        return -1;
    }
    return 0;
}

int main() {
    int server_fd, new_socket, epoll_fd, nfds;
    struct sockaddr_in address;
    struct epoll_event ev, events[MAX_EVENTS];
    char buffer[BUFFER_SIZE];
    socklen_t addrlen = sizeof(address);

    // Create server socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Start listening
    if (listen(server_fd, 10) < 0) {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Set server socket to non-blocking
    if (set_nonblocking(server_fd) < 0) {
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Create epoll instance
    if ((epoll_fd = epoll_create1(0)) == -1) {
        perror("epoll_create1");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Add the server socket to the epoll instance
    ev.events = EPOLLIN;
    ev.data.fd = server_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &ev) == -1) {
        perror("epoll_ctl: server_fd");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d...\n", PORT);

    while (1) {
        // Wait for events
        nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (nfds == -1) {
            perror("epoll_wait");
            break;
        }

        for (int i = 0; i < nfds; ++i) {
            if (events[i].data.fd == server_fd) {
                // Accept new connection
                while ((new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen)) > 0) {
                    printf("Accepted new connection: %d\n", new_socket);

                    // Set new socket to non-blocking
                    if (set_nonblocking(new_socket) < 0) {
                        close(new_socket);
                        continue;
                    }

                    // Add new socket to epoll
                    ev.events = EPOLLIN | EPOLLET;
                    ev.data.fd = new_socket;
                    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, new_socket, &ev) == -1) {
                        perror("epoll_ctl: new_socket");
                        close(new_socket);
                        continue;
                    }
                }

                if (new_socket == -1 && errno != EAGAIN && errno != EWOULDBLOCK) {
                    perror("accept");
                }
            } else {
                // Handle incoming data
                int client_fd = events[i].data.fd;
                int n = read(client_fd, buffer, BUFFER_SIZE);
                if (n > 0) {
                    // Echo data back to the client
                    write(client_fd, buffer, n);
                    printf("Write client fd: %d\n", client_fd);
                } else if (n == 0) {
                    // Connection closed
                    printf("Connection closed: %d\n", client_fd);
                    close(client_fd);
                } else if (errno != EAGAIN) {
                    perror("read");
                    close(client_fd);
                }
            }
        }
    }

    // Cleanup
    close(server_fd);
    close(epoll_fd);
    return 0;
}
