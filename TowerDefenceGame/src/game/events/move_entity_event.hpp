#pragma once
#include "game/events/event.hpp"
#include "game/world/entity.hpp"

namespace Event
{
    class MoveEntityEvent : public BaseEvent
    {
    private:
        Entity e; //ID of the first entity involved in the collision.
        float displacementX;
        float displacementY;
    public:
        MoveEntityEvent(Entity _e, float _disX, float _disY) : e(_e), displacementX(_disX), displacementY(_disY) {  };
        Entity get_entity() const { return e; }
        float get_displacement_x() const { return displacementX; }
        float get_displacement_y() const { return displacementY; }
    };
}
