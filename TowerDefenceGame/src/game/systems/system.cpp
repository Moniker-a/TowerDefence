#include "game/systems/system.hpp"

#include "game/world/world.hpp"
#include "game/world/component_type_register.hpp"

//Base class for all systems inherit from (allows polymorphism).
//A System is responsible for a specific part of game logic, for example movement, collision detection or draining health.
//They contain an update() function which performs their logic on a game entity. update() should check the entity mask against
//it's own system mast to ensure the entity contains the necessary components in order to be acted on by the system.

namespace System
{
}

