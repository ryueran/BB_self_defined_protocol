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

namespace sml = boost::sml;

tcp_client c;
// Define events
struct client_hello {};
struct server_ack1 {};
struct timeout {};

// Define action and conditions
const auto send_client_hello = [] {
    std::cout << "[Client] sends handshake request: ConnectMessage" << std::endl;
    // Step 1: send Connect Message
    ConnectMessage messages = {
        .type = MessageType::Connect,
        .size_pyld = 1
    };

    std::vector<uint8_t> con_msg = construct_connectMsg(messages);
    auto sent =  std::async(std::launch::async, [&] {
        return c.send_data(con_msg);
    });
    return sent.get();
};

const auto handle_server_ack1 = [](const auto&) {
    std::cout << "[Client] Receive response: ServerAck1" << std::endl;
    auto client_id = std::async(std::launch::async, [] {
        std::cout << "Handshake accomplished!" << std::endl;
        return c.receive(1024);
    });

    if(client_id.get() > 0)
    {
        return true;
    }
    return false;
};

const auto send_ack = [] {
    std::cout << "Timeout!" << std::endl;
    return true;
};

// Define state machine
struct ClientFSM {
    auto operator()() const {
        using namespace sml;
        return make_transition_table(
            *"established"_s + event<client_hello> / send_client_hello = "fin wait 1"_s,
            "fin wait 1"_s + event<server_ack1> [ handle_server_ack1 ] = "fin wait 2"_s,
            "timed wait"_s + event<timeout> / send_ack = X
        );
    }
};

enum class StateClient // should be integrated to clinet class later
{
    Idle,
    HandShaked
};

std::future<uint32_t> async_recv() {
    return std::async(std::launch::async, [] {
        return c.receive(1024);
    });
}

std::future<bool> async_send(const std::vector<uint8_t>& msg) {
    return std::async(std::launch::async, [&msg] {
        return c.send_data(msg);
    });
}

// void on_idle(StateClient& state)
// {
//     std::cout << "Client is idle" << std::endl;

//     // Step 1: send Connect Message
//     ConnectMessage messages = {
//         .type = MessageType::Connect,
//         .size_pyld = 1
//     };

//     std::vector<uint8_t> con_msg = construct_connectMsg(messages);

//     auto send_future = async_send(con_msg);
//     if (send_future.get()) {
//         std::cout << "Connect message sent!" << std::endl;
//         std::future<std::vector<uint8_t>> fut_msg = async_recv();
//         std::vector<uint8_t> msg = fut_msg.get();
//         if(msg[0] == static_cast<uint8_t>(MessageType::Accept) && state == StateClient::Idle)
//         {
//             state = StateClient::HandShaked;
//             std::cout << "Handshake with Server accomplished!" << std::endl;
//             // send StoreMessage
//         }
//         else
//         {
//             std::cout << "Received message: " << msg[0] << std::endl;
//         }
//     } else {
//         std::cerr << "Failed to send connect message!" << std::endl;
//         return;
//     }
// }

// void run(StateClient& state)
// {
//     while (true)
//     {
//         switch (state)
//         {
//             case StateClient::Idle:
//                 on_idle(state);
//                 break;
//             case StateClient::HandShaked:
//                 // Handle HandShaked state
//                 break;
//             default:
//                 break;
//         }
//     }
// }

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
    sml::sm<ClientFSM> fsm;

    // 模拟事件流
    fsm.process_event(client_hello{}); // Idle → Handshaked
    fsm.process_event(server_ack1{});  // Handshaked → MessageValued
    fsm.process_event(timeout{});  // MessageValued → 终止

    //done
    return 0;
}