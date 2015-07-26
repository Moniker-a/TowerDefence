#include "perception.hpp"
#include "game/components/locally_aware.hpp"
#include "game/components/position.hpp"
#include "game/utility/math2d.hpp"


//Example system.
//Updates the position of any entities which have velocity and position.
namespace System
{
    //Default constructor.just sets the system component mask from the component registry.
    Perception::Perception(EntityManager* _em, EventBus* _eb) : BaseSystem(_em, _eb)
    {
        systemMask.set(_em->get_component_id<Component::LocallyAware>());
        systemMask.set(_em->get_component_id<Component::Position>());
    }

    //System logic here... Updates an entity.
    void Perception::update()
    {
        for (Entity currentEntity=0; currentEntity < em->component_list_size(); currentEntity++) //For each entity...
        {
            if (em->match_mask(currentEntity, systemMask)) //If the system applies to the current entity, update it.
            {
                auto locallyAware = em->get_component<Component::LocallyAware>(currentEntity).lock();
                const auto curPosition = em->get_component<Component::Position>(currentEntity).lock();

                //First clear previous lists.
                locallyAware->localEntities.clear();
                if (locallyAware->cacheDistance)
                    locallyAware->get_distances().clear();
                if (locallyAware->cacheDirection)
                    locallyAware->get_directions().clear();

                //Now update local entity lists.
                for (Entity otherEntity=0; otherEntity < em->component_list_size(); otherEntity++) //For each other entitity.
                {
                    if (otherEntity == currentEntity) //Ignore self.
                        continue;

                    const auto otherPosition = em->get_component<Component::Position>(otherEntity).lock();
                    float distance = math2d::calc_distance(curPosition->get_x(), curPosition->get_y(), otherPosition->get_x(), otherPosition->get_y());

                    if (distance < locallyAware->radius)
                    {
                        locallyAware->localEntities.push_back(otherEntity);
                        if (locallyAware->cacheDistance)
                            locallyAware->get_distances().push_back(distance);
                        if (locallyAware->cacheDirection)
                            locallyAware->get_directions().push_back(math2d::calc_direction(curPosition->get_x(), curPosition->get_y(), otherPosition->get_x(), otherPosition->get_y()));
                    }
                }
            }
        }
    }
}
