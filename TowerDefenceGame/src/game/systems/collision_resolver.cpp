#include "collision_resolver.hpp"

#include <iostream> //temp...

namespace System
{
	void CollisionResolver::update(Entity _entityID, EntityManager& _em)
	{
		//Empty, not updated sequentially (or could be updated sequentially to actually detect collisions).
	}

	//Handle and resolve any CollisionEvents.
	void CollisionResolver::handle_event(const CollisionEvent &_event)
	{
		std::cout << "CollisionEvent received in ResolveCollision between entities " << _event.get_first() << " and " << _event.get_second() << ".\n";
	}
}
