#ifndef ACCEPTMESSAGE_HH
#define ACCEPTMESSAGE_HH

#include "MessageType.hh"
#include "stdint.h"

#pragma pack(push, 1)
struct AcceptMessage
{
    MessageType type;
    uint32_t size_pyld;
    uint32_t payload;
}; // size 9
#pragma pack(pop)

#endif