#pragma once
#include "game/systems/system.hpp"
#include "game/systems/event_handler.hpp"
#include "game/events/render_event.hpp"

namespace System
{
    class Render : public BaseSystem, public EventHandler<RenderEvent>
    {
    private:
        //
    public:
        Render(EntityManager* _em, EventBus* _eb);
        void update(Entity _entityID) {  } //Empty because rendering is entirely event driven.
        void handle_event(const RenderEvent &_event);
    };

}
