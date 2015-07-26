#include "debug.hpp"

#include <iostream>
#include "game/components/debug_info.hpp"

#include "game/components/position.hpp"

namespace System
{
    Debug::Debug(EntityManager* _em, EventBus* _eb, ResourceManager* _resourceManager) : BaseSystem(_em, _eb), resourceManager(_resourceManager)
    {
        systemMask.set(_em->get_component_id<Component::DebugInfo>());
    }

	void Debug::update()
	{
		for (Entity currentEntity=0; currentEntity < em->component_list_size(); currentEntity++) //For each entity...
        {
            if (em->match_mask(currentEntity, systemMask)) //If the system applies to the current entity, update it.
            {
                std::cout << "\tmask match!\n";
                const auto debugInfo = em->get_component<Component::DebugInfo>(currentEntity).lock();
                //if (debugInfo->name == "some name we're interested in...")
                //{
                    //Do stuff.
                    display_debug_info(currentEntity);
                //}
            }
        }
	}

	//Handle and resolve any TriggerEntityCreationEvents asynchronously.
	void Debug::handle_event(const Event::DebugInfoDisplay &_event)
	{
		std::cout << "Debug event: " << _event.optionalMessage << " (entity: " << _event.entity << ")" << std::endl;
		display_debug_info(_event.entity);
	}

	//Change as required while debugging.
	void Debug::display_debug_info(const Entity _entity) const
	{
	    const auto debugInfo = em->get_component<Component::DebugInfo>(_entity).lock();
	    std::cout << "Debug info for Entity " << _entity << "(debug name: " << debugInfo->name << ")\n";

	    const auto position = em->get_component<Component::Position>(_entity).lock();
	    std::cout << "Position: (" << position->x << ", " << position->y << ")" << std::endl;
	}
}
