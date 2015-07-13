#include "navigation.hpp"

#include "game/world/world.hpp"
#include "game/world/entity_manager.hpp"
#include "game/components/velocity.hpp"
#include "game/components/attractor.hpp"
#include "game/components/attractee.hpp"
#include "game/components/position.hpp"
#include <boost/lexical_cast.hpp>


#include <iostream> //temp

//System::Navigation is responsible for updating the velocity of entities.
namespace System
{
    //Default constructor.just sets the system component mask from the component registry.
    Navigation::Navigation(EntityManager* _em, EventBus* _eb) : BaseSystem(_em, _eb)
    {
        systemMask.set(_em->get_component_id<Component::Velocity>());
    }

    //System logic here... Updates an entity.
    void Navigation::update()
    {
        update_by_attractors();

        //
    }

    void Navigation::update_by_attractors()
    {
        //Get the Entity IDs of any entities with Attractors.
        std::list<Entity> attractorEntities;
        for (Entity currentEntity=0; currentEntity < em->component_list_size(); currentEntity++) //For each entity check to see if it's an Attractor.
            if (em->contains_component<Component::Attractor>(currentEntity))
                attractorEntities.push_back(currentEntity);

        //For each Attractee, adjust velocity based on proximity to each attractor.
        for (Entity currentEntity=0; currentEntity < em->component_list_size(); currentEntity++) //For each entity...
        {
            auto attracteeMask = systemMask;
            attracteeMask[em->get_component_id<Component::Attractee>()] = true; //Requires an Attractee component to be affected by Attractors.
            if (em->match_mask(currentEntity, attracteeMask)) //If the system applies to the current entity AND the entity is an Attractee, update it.
            {
                //Get current entity's position.
                const auto curPosition = em->get_component<Component::Position>(currentEntity).lock();

                //Calculate change in velocity for each attractor.
                float ddx = 0, ddy = 0;
                for (Entity attractorEntity : attractorEntities) //For each entity with an attractor.
                {
                    //TODO: The internals of this loop (calculating ddx and ddy could (should?) be handled by a function pointer.
                          //This way different behaviours could be stored as Resources (e.g. AttractorKernal), making code reusable for many types of distance based velocity change.
                          //Would also be much more efficient, as we only need to find and iterate over attractors and attractees once for all kinds distance based velocity change.
                          //Main disadvantage would be it may be awkward if an Entity needs to for example, both attract and repel objects.

                    if (attractorEntity == currentEntity) //Do not attract yourself.
                        continue;

                    //Get Attractor and Position of the current attractor entity.
                    const auto attractorsAttractor = em->get_component<Component::Attractor>(attractorEntity).lock();
                    const auto attractorsPosition = em->get_component<Component::Position>(attractorEntity).lock();
                    if (attractorsPosition == nullptr) //Throw exception if no position is located... Cannot be attracted to somewhere with no coordinates!
                        throw std::runtime_error("Logical error: Processing entity as an Attractor but it has no position.\n");

                    //Calculate distance to currentEntity. Multiply by Attractor and Attractee strengths.
                    float xDistance = attractorsPosition->get_x() - curPosition->get_x();
                    float yDistance = attractorsPosition->get_y() - curPosition->get_y();

                    //Get current entity's Attractee component.
                    auto curAttractee = em->get_component<Component::Attractee>(currentEntity).lock();

                    //Scale by Attractor and Attractee strength and update change in velocity.
                    ddx += attractorsAttractor->strength * curAttractee->strength * xDistance; //Should be square law?
                    ddy += attractorsAttractor->strength * curAttractee->strength * yDistance; //Should be square law

                    //Calculate direction of Attractor and adjust ddx and ddy accordingly.
                    //
                }

                //Adjust current entity's velocity accordingly.
                std::cout << "ddx = " << ddx << "\tddy = " << ddy << "\n";


                auto vel = em->get_component<Component::Velocity>(currentEntity).lock(); //Get a pointer to the position component of the entity corresponding to currentEntity.
                vel->set_dx(vel->get_dx()+ddx);
                vel->set_dy(vel->get_dy()+ddy);


                //Example move logic... Output to console for demonstration purposes.
                //std::cout << "Updating System::Navigation\n";
                //std::cout << "Old velocity:\t(" << vel->get_dx() << ", " << vel->get_dy() << ").\n";

                //Update Velocity here...


                //std::cout << "New velocity:\t(" << vel->get_dx() << ", " << vel->get_dy() << ").\n";
            }
        }
    }
}
