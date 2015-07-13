#include "wrap_detection.hpp"

#include "game/world/world.hpp"
#include "game/world/entity_manager.hpp"
#include "game/world/event_bus.hpp"
#include "game/components/position.hpp"
#include "game/components/velocity.hpp"
#include "game/components/bounding_box.hpp"
#include "game/events/move_entity_event.hpp"

#include <iostream> //Temp

//Example system.
//Updates the position of any entities which have velocity and position.
namespace System
{
    //Default constructor.just sets the system component mask from the component registry.
    WrapDetection::WrapDetection(EntityManager* _em, EventBus* _eb) : BaseSystem(_em, _eb)
    {

        systemMask.set(_em->get_component_id<Component::Position>());
        systemMask.set(_em->get_component_id<Component::BoundingBox>());
    }

    //System logic here... Updates an entity.
    void WrapDetection::update()
    {
        for (Entity currentEntity=0; currentEntity < em->component_list_size(); currentEntity++) //For each entity...
        {
            if (em->match_mask(currentEntity, systemMask)) //If the system applies to the current entity, update it.
            {
                auto bBox = em->get_component<Component::BoundingBox>(currentEntity).lock(); //Get pointer to the entity's bounding box.
                auto position = em->get_component<Component::Position>(currentEntity).lock(); //Get pointer to the entity's position.

                if (!bBox || !position) //Check these exist (they should do because SystemManager checks the entity's mask against the system mask.
                    throw std::runtime_error("WrapDetection::update() trying to access an entity's component which does not exist (but probably should)!");

                //If the position is outside the bounding box send a position_wrap_event.
                float x = position->get_x();
                float y = position->get_y();

                //If we need to wrap in any dimension, calculate displacement.
                float displacementX=0, displacementY=0;

                //X wrapping around bounding box.
                if (x < bBox->x1)
                    displacementX = bBox->x2-x;
                else if (x > bBox->x2)
                    displacementX = -(x-bBox->x1);

                //Y wrapping around bounding box.
                if (y < bBox->y1)
                    displacementY = bBox->y2-y;
                else if (y > bBox->y2)
                    displacementY = -(y-bBox->y1);

                if (displacementX != 0 || displacementY != 0)
                    eb->send_event<MoveEntityEvent>(MoveEntityEvent(currentEntity, displacementX, displacementY)); //This system doesn't have authority to move entities, so it sends an event.
            }
        }
    }
}
