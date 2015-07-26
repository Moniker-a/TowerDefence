#pragma once
#include "game/systems/system.hpp"
#include "game/systems/event_handler.hpp"
#include "game/events/render_event.hpp"

namespace System
{
    class Render : public BaseSystem, public EventHandler<Event::RenderEvent>
    {
    private:
        ResourceManager* resourceManager;
        unsigned int counter = 0; //Temp
    public:
        Render(EntityManager* _em, EventBus* _eb, ResourceManager* _resourceManager);
        void update() {  } //Empty because rendering is entirely event driven.
        void handle_event(const Event::RenderEvent &_event);
    };
}
