#pragma once

// internal headers
#include "game/systems/system.hpp"

// forward declarations
class ComponentManager;


namespace System
{
    //Updates the position of any entities which have position and velocity.
    class Movement : public System::BaseSystem
    {
      public:
        // constructor
        Movement();

        // preforms update
        void update(Entity _entityID, EntityManager& _em);
    };
}

