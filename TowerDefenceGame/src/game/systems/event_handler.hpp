#pragma once
#include "game/systems/event_handler_base.hpp"

class Event;

template <class EventType>
class EventHandler : public EventHandlerBase
{
private:
    //
public:
    virtual void handle_event(const EventType &_event)=0;
    virtual ~EventHandler() {  };
};
