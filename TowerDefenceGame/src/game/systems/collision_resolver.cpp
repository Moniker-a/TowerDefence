#include "collision_resolver.hpp"

#include <iostream> //temp...

namespace System
{
	void CollisionResolver::update()
	{
		//Empty, not updated sequentially (or could be updated sequentially to actually detect collisions).
	}

	//Handle and resolve any CollisionEvents.
	void CollisionResolver::handle_event(const Event::CollisionEvent &_event)
	{
	    //Currently not implemented. Was used to demonstrate the event system...
		std::cout << "CollisionEvent received in ResolveCollision between entities " << _event.get_first() << " and " << _event.get_second() << ".\n";
	}
}
