#include "entity_creation.hpp"

#include <iostream> //temp...

namespace System
{
    EntityCreation::EntityCreation(EntityManager* _em, EventBus* _eb, ResourceManager* _resourceManager) : BaseSystem(_em, _eb), resourceManager(_resourceManager)
    {
        //systemMask.set(_em->get_component_id<Component::Renderable>());
        //Mask should stay empty?
    }

	void EntityCreation::update()
	{
		//Empty, not updated sequentially (but could be if events simply added to an internal queue to be processed together?).
	}

	//Handle and resolve any TriggerEntityCreationEvents asynchronously.
	void EntityCreation::handle_event(const Event::TriggerEntityCreationEvent &_event)
	{
	    em->create_entity(_event.entitySpecification);
		std::cout << "TriggerEntityCreationEvent received in EntityCreation!\n";
	}
}
