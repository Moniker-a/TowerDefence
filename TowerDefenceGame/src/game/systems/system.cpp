#include "game/systems/system.hpp"

#include "game/world/world.hpp"
#include "game/world/component_type_register.hpp"

namespace System
{
    //Base System from which all other Systems are derived to allow polymorphism.
    BaseSystem::BaseSystem() : mask(World::get_registry().get_size(), false)
    {  }
}

