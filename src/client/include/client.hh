#ifndef CLIENT_HH
#define CLIENT_HH

#include<iostream>    //cout
#include<stdio.h> //printf
#include<string.h>    //strlen
#include<string>  //string
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr
#include<netdb.h> //hostent
#include<vector>

using namespace std;
 
/**
    TCP Client class
*/
class tcp_client
{
private:
    int sock;
    std::string address;
    int port;
    struct sockaddr_in server;
     
public:
    tcp_client();
    bool conn(string, int);
    bool send_data(const std::vector<uint8_t>& data);
    std::vector<uint8_t> receive(int);
};
 
tcp_client::tcp_client()
{
    sock = -1;
    port = 0;
    address = "";
}
 
/**
    Connect to a host on a certain port number
*/
bool tcp_client::conn(string address , int port)
{
    //create socket if it is not already created
    if(sock == -1)
    {
        //Create socket
        sock = socket(AF_INET , SOCK_STREAM , 0);
        if (sock == -1)
        {
            perror("Could not create socket");
        }
         
        cout<<"Socket created\n";
    }
    else    {   /* OK , nothing */  }
    
    uint32_t invalid_addr = (1ULL << 32) - 1;;
    //setup address structure
    if(inet_addr(address.c_str()) == invalid_addr)
    {
        struct hostent *he;
        struct in_addr **addr_list;
         
        //resolve the hostname, its not an ip address
        if ( (he = gethostbyname( address.c_str() ) ) == NULL)
        {
            //gethostbyname failed
            herror("gethostbyname");
            cout<<"Failed to resolve hostname\n";
             
            return false;
        }
         
        //Cast the h_addr_list to in_addr , since h_addr_list also has the ip address in long format only
        addr_list = reinterpret_cast<in_addr **>(he->h_addr_list); // here is dangerous, has to be carefull
 
        for(int i = 0; addr_list[i] != NULL; i++)
        {
            //strcpy(ip , inet_ntoa(*addr_list[i]) );
            server.sin_addr = *addr_list[i];
             
            cout<<address<<" resolved to "<<inet_ntoa(*addr_list[i])<<endl;
             
            break;
        }
    }
     
    //plain ip address
    else
    {
        server.sin_addr.s_addr = inet_addr( address.c_str() );
    }
     
    server.sin_family = AF_INET;
    server.sin_port = htons( port );
     
    //Connect to remote server
    if (connect(sock , reinterpret_cast<sockaddr *>(&server) , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }
     
    cout<<"Connected\n";
    return true;
}
 
/**
    Send data to the connected host
*/
bool tcp_client::send_data(const std::vector<uint8_t>& data)
{
    //Send some data
    if( send(sock , data.data() , data.size() , 0) < 0)
    {
        perror("Send failed : ");
        return false;
    }
    cout<<"Data send\n";
     
    return true;
}
 
/**
    Receive data from the connected host
*/
std::vector<uint8_t> tcp_client::receive(int size=512)
{
    std::vector<uint8_t> buffer(size, 0);
     
    //Receive a reply from the server
    ssize_t size_msg = recv(sock , buffer.data(), sizeof(buffer) , 0);
    if(size_msg < 0)
    {
        puts("recv failed");
    }

    buffer.resize(size_msg);
    uint32_t client_id = buffer[buffer.size() - 3] | buffer[buffer.size() - 2] | buffer[buffer.size() - 1] | buffer[buffer.size() - 0];
    std::cout << "client id is: " << unsigned(client_id) << std::endl;
    return buffer;
}

#endif