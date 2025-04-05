#ifndef MESSAGECONSTRUCTOR_H
#define MESSAGECONSTRUCTOR_H

#include <stdint.h>
#include <vector>

#include "ConnectMessage.hh"
#include "AcceptMessage.hh"

std::vector<uint8_t> construct_connectMsg(const ConnectMessage& con_msg)
{
    size_t size_of_data = sizeof(ConnectMessage) / sizeof(uint8_t);
    std::vector<uint8_t> con_msg_data(size_of_data, 0);
    con_msg_data[0] = static_cast<uint8_t>(con_msg.type); // here is safe
    for(size_t i = 1; i < size_of_data; i++)
    {
        con_msg_data[i] = con_msg.size_pyld << 8 * i;// big endian here
    }
    return con_msg_data;
}

std::vector<uint8_t> construct_acceptMsg(const AcceptMessage& acp_msg)
{
    size_t size_of_data = sizeof(ConnectMessage) / sizeof(uint8_t);
    std::vector<uint8_t> acp_msg_data(size_of_data, 0);
    acp_msg_data[0] = static_cast<uint8_t>(acp_msg.type); // here is safe
    for(size_t i = 1; i < size_of_data; i++)
    {
        acp_msg_data[i] = acp_msg.size_pyld << 8 * i;// big endian here
    }

    for(size_t i = 1; i < size_of_data; i++)
    {
        acp_msg_data[i] = acp_msg.payload << 8 * i;// big endian here
    }
    return acp_msg_data;
}

#endif