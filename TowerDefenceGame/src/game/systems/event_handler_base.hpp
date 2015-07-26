#pragma once

class BaseEvent;

//Provides an polymorphism for the event handling interface.
class EventHandlerBase
{
private:
public:
    virtual ~EventHandlerBase() { }
};
