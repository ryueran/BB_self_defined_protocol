#ifndef CONNECTMESSAGE_H
#define CONNECTMESSAGE_H

#include "MessageType.hh"
#include "stdint.h"

#pragma pack(push, 1)
struct ConnectMessage
{
    MessageType type;
    uint32_t size_pyld;
};
#pragma pack(pop)

#endif