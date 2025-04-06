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

#include "MessageType.hh"
#include "ConnectMessage.hh"
#include "utility/MessageConstructor.hh"
#include "client.hh"

tcp_client c;

enum class StateClient // should be integrated to clinet class later
{
    Idle,
    HandShaked
};

// void read_event(std::future<std::vector<uint8_t>>& msg, StateClient& state)
// {
//     if(msg.get()[0] == static_cast<uint8_t>(MessageType::Accept) && state == StateClient::Idle)
//     {
//         state = StateClient::HandShaked;
//         std::cout << "Handshake with Server accomplished!" << std::endl;
//         // send StoreMessage
//     }

//     while(1)
//     {
        
//     }
// }
std::future<std::vector<uint8_t>> async_recv() {
    return std::async(std::launch::async, [] {
        return c.receive(1024);
    });
}

void on_idle(StateClient& state)
{
    std::cout << "Client is idle" << std::endl;
    std::future<std::vector<uint8_t>> fut_msg = async_recv();
    std::vector<uint8_t> msg = fut_msg.get();
    if(msg[0] == static_cast<uint8_t>(MessageType::Accept) && state == StateClient::Idle)
    {
        state = StateClient::HandShaked;
        std::cout << "Handshake with Server accomplished!" << std::endl;
        // send StoreMessage
    }
    else
    {
        std::cout << "Received message: " << msg[0] << std::endl;
    }
}

void run(StateClient& state)
{
    while (true)
    {
        switch (state)
        {
            case StateClient::Idle:
                on_idle(state);
                break;
            case StateClient::HandShaked:
                // Handle HandShaked state
                break;
            default:
                break;
        }
    }
}

int main()
{

    std::promise<std::vector<uint8_t>> prom_msg;
    std::future<std::vector<uint8_t>> fut_msg = prom_msg.get_future();

    // start state machine of client
    StateClient state = StateClient::Idle;

    // Step 1: send Connect Message
    ConnectMessage messages = {
        .type = MessageType::Connect,
        .size_pyld = 1
    };

    std::vector<uint8_t> con_msg = construct_connectMsg(messages);

    //connect to host
    c.conn("localhost" , 8080);
     
    //send some data
    c.send_data(con_msg);
     
    //receive and echo reply
    cout<<"----------------------------\n\n";
    run(state);
    cout<<"\n\n----------------------------\n\n";

    //done
    return 0;
}