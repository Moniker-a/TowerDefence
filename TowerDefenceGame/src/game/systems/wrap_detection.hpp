#pragma once

#include "game/systems/system.hpp"
#include "game/systems/event_handler.hpp"
#include "game/events/move_entity_event.hpp"

namespace System
{
    //Updates generates wrap events if an object moves outside it's bounding box.
    class WrapDetection : public System::BaseSystem
    {
      public:
        WrapDetection(EntityManager* _em, EventBus* _eb);
        void update(Entity _entityID);
    };
}

