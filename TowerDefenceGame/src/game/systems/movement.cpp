#include "movement.hpp"

#include "game/world/world.hpp"
#include "game/world/entity_manager.hpp"
#include "game/components/position.hpp"
#include "game/components/velocity.hpp"


#include <iostream>

//Example system.
//Updates the position of any entities which have velocity and position.
namespace System
{
    //Default constructor.just sets the system component mask from the component registry.
    Movement::Movement()
    {
       mask.set(World::get_registry().get_ID<Component::Position>());
       mask.set(World::get_registry().get_ID<Component::Velocity>());
    }

    //System logic here... Updates an entity.
    void Movement::update(Entity _entityID, EntityManager& _em)
    {
        if (auto pos = _em.get_component<Component::Position>(_entityID).lock()) //Get a pointer to the position component of the entity corresponding to _entityID.
        {
            //Example move logic... Output to console for demonstration purposes.
            std::cout << "Updating System::Movement\n";
            std::cout << "Old position:\t(" << pos->get_x() << ", " << pos->get_y() << ").\n";
            pos->set_x(pos->get_x()+1);
            pos->set_y(pos->get_y()+1);
            std::cout << "New position:\t(" << pos->get_x() << ", " << pos->get_y() << ").\n";
        }
        else
            throw std::runtime_error("Movement::update() trying to access a Position component when it has been deleted!");
    }
}
