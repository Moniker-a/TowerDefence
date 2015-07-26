#pragma once

#include "game/systems/system.hpp"
#include "game/systems/event_handler.hpp"
#include "game/events/debug_info_display.hpp"

class ResourceManager;

namespace System
{
    //System::Navigation is responsible for updating the velocity of entities.
    class Debug : public System::BaseSystem//, public EventHandler<MoveEntityEvent>
    {
    private:
        ResourceManager* resourceManager;
    public:
        // constructor
        Debug(EntityManager* _em, EventBus* _eb, ResourceManager* resourcemanager);

        void update();
        void handle_event(const Event::BaseEvent &_event) {};
		void handle_event(const Event::DebugInfoDisplay &_event);

		void display_debug_info(const Entity _entity) const;
    };
}


