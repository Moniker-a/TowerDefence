#pragma once
#include "game/systems/system.hpp"
#include "game/systems/event_handler.hpp"
#include "game/events/trigger_entity_creation_event.hpp"

//Entirely event driven system for creating entities.
namespace System
{
	class EntityCreation : public System::BaseSystem, public EventHandler<Event::TriggerEntityCreationEvent>
	{
	private:
	    ResourceManager* resourceManager;
	public:
	    EntityCreation(EntityManager* _em, EventBus* _eb, ResourceManager* _resourceManager);
		void update();
		void handle_event(const Event::BaseEvent &_event) {};
		void handle_event(const Event::TriggerEntityCreationEvent &_event);
	};
}
