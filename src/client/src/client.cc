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

int main()
{
    tcp_client c;
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
    c.init_fms();
    std::future<void> consume = std::async(std::launch::async | std::launch::deferred, [&] () 
        {
            while (true) {
                c.run_fms();
            }
        }
    );

    std::future<void> produce = std::async(std::launch::async, [&] () 
        {
            while (true) {
                c.read_event_fms();
            }
        }
    );

    //done
    return 0;
}