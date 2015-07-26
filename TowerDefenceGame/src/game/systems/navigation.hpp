#pragma once

#include "game/systems/system.hpp"
#include "game/systems/event_handler.hpp"
#include "game/events/move_entity_event.hpp"

namespace System
{
    //System::Navigation is responsible for updating the velocity of entities.
    class Navigation : public System::BaseSystem//, public EventHandler<MoveEntityEvent>
    {
    private:
        void update_attractees(); //Updates the velocity of entities containing the 'Attractee' component based on proximity and position of attractors.
        void update_boids(); //Updates the velocity of entities containing the 'Boid' component.
        void self_propel();

        void clip_to_max_velocity(); //Scales down any velocity which exceeds the maximum velocity.

    public:
        // constructor
        Navigation(EntityManager* _em, EventBus* _eb);

        void update();
        //void handle_event(const Event &_event) {};
		//void handle_event(const MoveEntityEvent &_event);
    };
}

