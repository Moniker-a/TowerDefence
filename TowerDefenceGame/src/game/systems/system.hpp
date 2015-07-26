#pragma once

#include <boost/dynamic_bitset.hpp>
//#include "game/world/world.hpp"
#include "game/world/component_type_register.hpp"
#include "game/world/entity.hpp"
#include "game/world/entity_manager.hpp"

class EventBus;

namespace System
{
    //Base class for all systems inherit from (allows polymorphism).
    //A System is responsible for a specific part of game logic, for example movement, collision detection or draining health.
    //They contain an update() function which performs their logic on a game entity. update() should check the entity mask against
    //it's own system mast to ensure the entity contains the necessary components in order to be acted on by the system.
    class BaseSystem
    {
    protected:
        EntityManager* em; //A pointer to the entity manager that the system will act on.
        EventBus* eb; //A pointer to the event bus that the system sends events to.
        boost::dynamic_bitset<> systemMask; //Mask specifying which Components the system uses. Bit position corresponds to the component registry ComponentID

    public:
        BaseSystem(EntityManager* _em, EventBus* _eb=nullptr) : em(_em), eb(_eb), systemMask(_em->num_component_types(), false) {  };
        virtual void update()=0; //Updates an entity.
        const boost::dynamic_bitset<> get_system_mask() const { return systemMask; }; //Returns the system's component mask.
        virtual ~BaseSystem() {  }; //Required.
    };
}

