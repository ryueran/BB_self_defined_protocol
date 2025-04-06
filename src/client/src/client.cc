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

enum class StateClient // should be integrated to clinet class later
{
    Init,
    HandShaked
};

void read_event(std::future<std::vector<uint8_t>>& msg, StateClient& state)
{
    if(msg.get()[0] == static_cast<uint8_t>(MessageType::Accept))
    {
        state = StateClient::HandShaked;
        std::cout << "Handshake with Server accomplished!" << std::endl;
        // send StoreMessage
    }
}

int main()
{

    tcp_client c;
    std::promise<std::vector<uint8_t>> prom_msg;
    std::future<std::vector<uint8_t>> fut_msg = prom_msg.get_future();

    // start state machine of client
    StateClient state = StateClient::Init;

    std::thread th1(read_event, std::ref(fut_msg), std::ref(state));
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
    prom_msg.set_value(c.receive(1024));
    cout<<"\n\n----------------------------\n\n";

    th1.join();
    //done
    return 0;
}