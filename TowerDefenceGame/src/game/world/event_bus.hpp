#pragma once
#include <set>
#include <unordered_map>
#include <typeindex>
#include "game/events/event.hpp"
#include "game/systems/system.hpp"
#include "game/systems/event_handler.hpp"

//class EventHandlerBase;
namespace System
{
    class BaseSystem;
}

class EventBus
{
private:
    std::unordered_map<std::type_index, std::set<EventHandlerBase*>> subscriberMap; //Maps event types to systems. More than one system can be to receive the same event type.
public:
    template <typename EventType>
    void add_subscriber(System::BaseSystem* _system); //Set _system to receive events of type EventType

    template <typename EventType>
    void remove_subscriber(EventHandlerBase& _system); //Set _system to no longer receives events of type EventType

    template <typename EventType>
    void send_event(EventType _event); //Sends an event to any system subscribing to said event type...
};

//Set _system to receive events of type EventType
template <typename EventType>
void EventBus::add_subscriber(System::BaseSystem* _system)
{
    //Convert _system to EventHandlerBase.
    EventHandlerBase* subscribee = dynamic_cast<EventHandlerBase*>(_system);

    //Check if subscriber list for EventType exists.
    if (subscriberMap.count(typeid(EventType))) ////If it does add _system to the subscriber set.
        subscriberMap[typeid(EventType)].insert(&(*subscribee));
    else //If this is the first time anything has subscribed to this EventType create the event set and add _system.
        subscriberMap[typeid(EventType)].insert(&(*subscribee));

    //Check if subscriber list for EventType exists.
    //if (subscriberMap.count(typeid(EventType))) ////If it does add _system to the subscriber set.
    //    subscriberMap[typeid(EventType)].insert(&(*_system));
    //else //If this is the first time anything has subscribed to this EventType create the event set and add _system.
    //    subscriberMap[typeid(EventType)].insert(&(*_system));
}

//Set _system to no longer receives events of type EventType
template <typename EventType>
void EventBus::remove_subscriber(EventHandlerBase& _system)
{
    auto subscriberSet = subscriberMap.find(typeid(EventType)); //Find the correct subscriberSet for this EventType.
    if (subscriberSet != subscriberMap.end()) //If EventType has any subscribers.
    {
        auto entry = subscriberSet->second.find(&_system); //Find the system's entry.
        if (entry != subscriberSet->second.end()) //If system is actually registered to the EventType.
            subscriberSet->second.erase(entry); //Unsubscribe the system.
    }
}

//Sends an event to any system subscribing to said event type...
template <typename EventType>
void EventBus::send_event(EventType _event)
{
    //For every subscriber in the event type's subscriber set...
    for (auto itr : subscriberMap[typeid(_event)]) //itr is an iterator for the subscriber set.
        static_cast<EventHandler<EventType>*>(&(*itr))->handle_event(_event); //Cast to correct type and handle the event!
}
