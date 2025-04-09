/**
    C++ client example using sockets
*/
#include<iostream>
#include<stdio.h>
#include<string.h>
#include<string>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<vector>
#include<thread>
#include<future>
#include<sml/include/boost/sml.hpp>

#include "MessageType.hh"
#include "ConnectMessage.hh"
#include "utility/MessageConstructor.hh"
#include "client.hh"



tcp_client c;

typedef enum
{
    Client_Id_Accepted,
    // other events...
} Event;

typedef enum {
    Idle,
    HandShaked,
    // other message types...
} ClientState;

struct ClientSwitcher {
    ClientState state;
};

void ClientSwitcher_Feed(struct ClientSwitcher* state, Event event)
{
    switch (state->state)
    {
        case Idle:
            if (event == Client_Id_Accepted)
            {
                std::cout << "Client is HandShaked" << std::endl;
                state->state = HandShaked;
            }
            break;
        case HandShaked:
            // Handle HandShaked state
            break;
        default:
            break;
    }
}

std::future<std::vector<uint8_t>> async_recv() {
    return std::async(std::launch::async, [] {
        return c.receive(1024);
    });
}

std::future<bool> async_send(const std::vector<uint8_t>& msg) {
    return std::async(std::launch::async, [&msg] {
        return c.send_data(msg);
    });
}

void run(struct ClientSwitcher* state)
{
    // Run state machine
    std::future<std::vector<uint8_t>> buffer_recv = async_recv();
    auto msg = buffer_recv.get();
    uint32_t client_id = msg[msg.size() - 3] | msg[msg.size() - 2] | msg[msg.size() - 1] | msg[msg.size() - 0];
    std::cout << "client id is: " << unsigned(client_id) << std::endl;
    if(msg[0] == static_cast<uint8_t>(MessageType::Accept))
    {
        ClientSwitcher_Feed(state, Client_Id_Accepted);
    }

}

void ClientState_Init(struct ClientSwitcher* state)
{
    std::cout << "Client is idle" << std::endl;
    state->state = Idle;

    // Step 1: send Connect Message
    ConnectMessage messages = {
        .type = MessageType::Connect,
        .size_pyld = 1
    };
    std::vector<uint8_t> con_msg = construct_connectMsg(messages);
    c.send_data(con_msg);
    std::cout << "Connect message sent!" << std::endl;
}



int main()
{

    std::promise<std::vector<uint8_t>> prom_msg;
    std::future<std::vector<uint8_t>> fut_msg = prom_msg.get_future();

    // start state machine of client
    // StateClient state = StateClient::Idle;


    //connect to host
    c.conn("localhost" , 8080);
    std::cout<<"Connected to server\n";
     
    //receive and echo reply
    // cout<<"----------------------------\n\n";
    // run(state);
    // cout<<"\n\n----------------------------\n\n";
    struct ClientSwitcher fsm;
    ClientState_Init(&fsm);

    run(&fsm);

    //done
    return 0;
}