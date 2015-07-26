#pragma once

#include "game/systems/system.hpp"
#include "game/systems/event_handler.hpp"

namespace System
{
    //Updates the local awareness context (list of nearby entities, their distances etc.) for objects which have LocallyAware
    class Perception : public System::BaseSystem
    {
    public:
        Perception(EntityManager* _em, EventBus* _eb=nullptr);
        void update();
    };
}


