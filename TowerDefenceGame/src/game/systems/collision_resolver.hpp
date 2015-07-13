#pragma once
#include "game/systems/system.hpp"
#include "game/systems/event_handler.hpp"
#include "game/events/collision_event.hpp"

//NOT YET IMPLEMENTED...
namespace System
{
	class CollisionResolver : public System::BaseSystem, public EventHandler<CollisionEvent>
	{
	private:
	public:
		void update();
		void handle_event(const Event &_event) {};
		void handle_event(const CollisionEvent &_event);
	};
}
