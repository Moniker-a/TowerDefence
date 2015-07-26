#include "movement.hpp"

#include "game/world/entity_manager.hpp"
#include "game/components/position.hpp"
#include "game/components/velocity.hpp"
#include "game/components/bounding_box.hpp"
#include <boost/lexical_cast.hpp>

#include <cmath>

#include <iostream> //Temp

//Example system.
//Updates the position of any entities which have velocity and position.
namespace System
{
    //Default constructor.just sets the system component mask from the component registry.
    Movement::Movement(EntityManager* _em, EventBus* _eb) : BaseSystem(_em, _eb)
    {
        systemMask.set(_em->get_component_id<Component::Position>());
        systemMask.set(_em->get_component_id<Component::Velocity>());
    }

    //System logic here... Updates an entity.
    void Movement::update()
    {
        for (Entity currentEntity=0; currentEntity < em->component_list_size(); currentEntity++) //For each entity...
        {
            if (em->match_mask(currentEntity, systemMask)) //If the system applies to the current entity, update it.
            {

                auto pos = em->get_component<Component::Position>(currentEntity).lock(); //Get a pointer to the position component of the entity corresponding to currentEntity.
                const auto vel = em->get_component<Component::Velocity>(currentEntity).lock(); //Get a pointer to the position component of the entity corresponding to currentEntity.

                //Move logic...
                pos->set_x(pos->get_x()+vel->get_dx());
                pos->set_y(pos->get_y()+vel->get_dy());
            }
        }
    }

    //Handle a PositionWrapEvent
    void Movement::handle_event(const Event::MoveEntityEvent &_event)
    {
        //Extract the entity which the event concerns.
        Entity entity = _event.get_entity();

        //Check entity has the required components.
        if (em->match_mask(entity, systemMask))
            //(em->get_entity_mask(entity) & systemMask) == systemMask)
        {
            std::cout << "entity " << _event.get_entity() << "'s position will now be clipped.\n";

            //Adjust position so that it wraps.
            auto pos = em->get_component<Component::Position>(entity).lock();
            if (pos = em->get_component<Component::Position>(entity).lock())
            {
                //Update x and y positions using the displacement quantities from _event.
                pos->set_x(pos->get_x()+_event.get_displacement_x());
                pos->set_y(pos->get_y()+_event.get_displacement_y());
                std::cout << "Position after MoveEntityEvent:\t(" << pos->get_x() << ", " << pos->get_y() << ").\n";
            }
            else
                throw std::runtime_error("WrapDetection::update() trying to access an entity's component which does not exist (but probably should)!");

        }
        else
            throw std::runtime_error("Movement::handle_event(PositionWrapEvent) - Cannot handle event on entity which doesn't match system mask.");
    }
}
