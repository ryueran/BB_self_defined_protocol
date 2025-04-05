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

#include "MessageType.hh"
#include "ConnectMessage.hh"
#include "utility/MessageConstructor.hh"
#include "client.hh"

int main()
{
    tcp_client c;
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
    cout << unsigned(c.receive(1024)[0]);
    cout<<"\n\n----------------------------\n\n";
     
    //done
    return 0;
}