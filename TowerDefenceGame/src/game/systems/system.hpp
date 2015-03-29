#pragma once

// standard headers
#include <boost/dynamic_bitset.hpp>
#include "game/world/entity.hpp"

class EntityManager;

namespace System
{
    //Base class for all systems inherit from (allows polymorphism).
    class BaseSystem
    {
      protected:
        boost::dynamic_bitset<> mask; //Mask specifying which Components the system uses. Bit position corresponds to the component registry ComponentID

      public:
        BaseSystem();
        virtual void update(Entity _entityID, EntityManager& _em)=0; //Updates an entity.
        const boost::dynamic_bitset<> get_system_mask() const { return mask; }; //Returns the system's component mask.
        virtual ~BaseSystem() {  }; //Required.
    };
}

