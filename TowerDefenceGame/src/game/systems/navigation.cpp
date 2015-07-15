#include "navigation.hpp"

#include "game/world/world.hpp"
#include "game/world/entity_manager.hpp"
#include "game/components/velocity.hpp"
#include "game/components/attractor.hpp"
#include "game/components/attractee.hpp"
#include "game/components/position.hpp"
#include "game/components/boid.hpp"
#include "game/utility/math2d.hpp"
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
        //Call various update subroutines...
        update_attractees();
        update_boids();
    }

    //Updates the velocity of any entity containing the 'Attractee' component.
    void Navigation::update_attractees()
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
            }
        }
    }

    void Navigation::update_boids()
    {
        //Get the Entity IDs of any entities with Boid components.
        std::list<Entity> boidEntities;
        for (Entity currentEntity=0; currentEntity < em->component_list_size(); currentEntity++) //For each entity check to see if it's a Boid.
            if (em->contains_component<Component::Boid>(currentEntity))
                boidEntities.push_back(currentEntity);

        //For each Boid entity, adjust velocity based on proximity to each other Boid entity.
        for (Entity currentEntity : boidEntities)
        {
            //Get current entity's position.
            auto curPosition = em->get_component<Component::Position>(currentEntity).lock();
            const auto curBoidComponent = em->get_component<Component::Boid>(currentEntity).lock();

            float dTheta = 0.0; //Tracks change in orientation.
            //float ddx = 0.0; //Tracks change in x velocity.
            //float ddy = 0.0; //Tracks change in y velocity.
            for (Entity otherBoidEntity : boidEntities) //For every other boid entity, we might need to update currentEntity's velocity...
            {
                if (otherBoidEntity == currentEntity) //Ignore self.
                    continue;

                //Calculate distance between currentEntity and boidEntity.
                const auto othersPosition = em->get_component<Component::Position>(otherBoidEntity).lock();
                const float xDistance = othersPosition->get_x() - curPosition->get_x();
                const float yDistance = othersPosition->get_y() - curPosition->get_y();

                const float distance = vector_magnitude(xDistance, yDistance); //Distance between the two boids (always positive or 0).
                const float direction = cartesian_to_polar(xDistance, yDistance).second;

                //Update the currentEntity's velocity if within attraction radius.
                if (distance <= curBoidComponent->attractRadius)
                {
                    float orientationDifference;
                    if (std::abs(curPosition->get_orientation() - direction) < std::abs(direction - curPosition->get_orientation()))
                        orientationDifference = curPosition->get_orientation() - othersPosition->get_orientation();
                    else
                        orientationDifference = direction - curPosition->get_orientation();

                    dTheta += orientationDifference*curBoidComponent->attractStrength;
                }

                //Update the currentEntity's velocity if within repel radius.
                if (distance <= curBoidComponent->repelRadius)
                {
                    float orientationDifference;
                    if (std::abs(curPosition->get_orientation() - direction) < std::abs(direction - curPosition->get_orientation()))
                        orientationDifference = curPosition->get_orientation() - othersPosition->get_orientation();
                    else
                        orientationDifference = direction - curPosition->get_orientation();

                    dTheta -= orientationDifference*curBoidComponent->repelStrength;
                }

                //Update the currentEntity's velocity if within reorient radius.
                if (distance <= curBoidComponent->reorientRadius)
                {
                    //Calculate the difference in orientation between the two Boid entities
                    float orientationDifference;
                    if (std::abs(curPosition->get_orientation() - othersPosition->get_orientation()) < std::abs(othersPosition->get_orientation() - curPosition->get_orientation()))
                        orientationDifference = curPosition->get_orientation() - othersPosition->get_orientation();
                    else
                        orientationDifference = othersPosition->get_orientation() - curPosition->get_orientation();

                    //Update change in orientation.
                    dTheta += orientationDifference*curBoidComponent->reorientStrength;
                }
            }

            //Update velocity and orientation.
            auto curVelocity = em->get_component<Component::Velocity>(currentEntity).lock();
            curPosition->set_orientation(curPosition->get_orientation()+dTheta);

            //Accelerate forward (TODO: should be in self-propelling component?)
            auto newVelocity = polar_to_cartesian(curBoidComponent->maxVelocity, curPosition->get_orientation());
            curVelocity->set_dx(newVelocity.first);
            curVelocity->set_dy(newVelocity.second);
        }
    }
}
