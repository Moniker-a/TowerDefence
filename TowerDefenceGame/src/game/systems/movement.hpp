#pragma once

#include "game/systems/system.hpp"
#include "game/systems/event_handler.hpp"
#include "game/events/move_entity_event.hpp"

namespace System
{
    //Updates the position of any entities which have position and velocity.
    class Movement : public System::BaseSystem, public EventHandler<Event::MoveEntityEvent>
    {
    public:
        // constructor
        Movement(EntityManager* _em, EventBus* _eb);

        // preforms update
        void update();//, EntityManager& _em);
        void handle_event(const Event::BaseEvent &_event) {};
		void handle_event(const Event::MoveEntityEvent &_event);
    };
}

