#ifndef MESSAGETYPE_H
#define MESSAGETYPE_H

enum class MessageType: uint8_t 
{
    Unknown,
    Connect,
    Accept,
    Error,
    Store,
    Load,
    Value
};

#endif