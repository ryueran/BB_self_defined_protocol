#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <iostream>
#include <vector>
#include <thread>
#include <future>

#include "MessageType.hh"
#include "ConnectMessage.hh"
#include "utility/MessageConstructor.hh"
#include "client.hh"

typedef enum
{
    Client_Id_Accepted,
    // other events...
} Event;

typedef enum {
    Idle,
    HandShaked,
    // other message types...
} ClientState;

struct ClientSwitcher {
    ClientState state;
};

class State
{
public:
    ClientSwitcher fsm;
    State() : fsm{Idle} {
        std::cout << "Client is idle" << std::endl;
        fsm.state = Idle;
    }
    void ClientSwitcher_event_produce(Event event)
    {
        events.push_back(event);
    }

    void ClientSwitcher_event_consume(struct ClientSwitcher* state)
    {
        Event event;
        if (events.size() > 0)
        {
            event = events.back();
            events.pop_back();
        }
        else
        {
            return;
        }

        switch (state->state)
        {
            case Idle:
                if (event == Client_Id_Accepted)
                {
                    state->state = HandShaked;
                }
                break;
            case HandShaked:
                // Handle HandShaked state
                break;
            default:
                break;
        }
    }

private:
    std::vector<Event> events = {};
};

#endif