#pragma once
#include "game/events/event.hpp"
#include "game/world/entity.hpp"

namespace Event
{
    class CollisionEvent : public BaseEvent
    {
    private:
        Entity e1; //ID of the first entity involved in the collision.
        Entity e2; //ID of the second entity involved in the collision.
    public:
        CollisionEvent(Entity _e1, Entity _e2) : e1(_e1), e2(_e2) {  };
        Entity get_first() const { return e1; }
        Entity get_second() const { return e2; }
    };
}
