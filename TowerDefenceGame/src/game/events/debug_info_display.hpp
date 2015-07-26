#pragma once
#include "game/events/event.hpp"
#include "game/world/entity.hpp"

namespace Event
{
    struct DebugInfoDisplay : public BaseEvent
    {
        Entity entity; //ID of the first entity involved in the collision.
        std::string optionalMessage;
        DebugInfoDisplay(Entity _entity, std::string _optionalMessage="") : entity(_entity), optionalMessage(_optionalMessage) {  };
    };
}
