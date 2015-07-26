#include "navigation.hpp"

#include "game/world/entity_manager.hpp"
#include "game/components/velocity.hpp"
#include "game/components/attractor.hpp"
#include "game/components/attractee.hpp"
#include "game/components/locally_aware.hpp"
#include "game/components/position.hpp"
#include "game/components/boid.hpp"
#include "game/components/self_propelled.hpp"
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
        //Could be optimised by creating a list of velocity and/or position Components, then passing a reference to these to the various helper subroutines?

        //Call various update subroutines...
        update_attractees();
        update_boids();
        self_propel();

        //Adjusts velocity if exceeding maximum velocity.
        clip_to_max_velocity();
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
        for (Entity currentEntity=0; currentEntity < em->component_list_size(); currentEntity++) //For each entity check to see if it's a Boid and locally away (makes no sense for a boid to not be away of its local environment).
            if (em->contains_component<Component::Boid>(currentEntity) && em->contains_component<Component::LocallyAware>(currentEntity))
                boidEntities.push_back(currentEntity);

        //For each Boid entity, adjust velocity based on proximity to each other Boid entity.
        for (Entity currentEntity : boidEntities)
        {
            //Get current entity's position.
            auto curPosition = em->get_component<Component::Position>(currentEntity).lock();
            const auto curBoidComponent = em->get_component<Component::Boid>(currentEntity).lock();
            const auto localAwareness = em->get_component<Component::LocallyAware>(currentEntity).lock();
            float dTheta = 0.0; //Tracks change in orientation.

            for (unsigned int i=0; i<localAwareness->localEntities.size(); i++) //For every boid in the current boid's local awareness, we might need to update currentEntity's velocity...
            {
                Entity otherBoidEntity = localAwareness->localEntities[i];

                if (!em->contains_component<Component::Boid>(otherBoidEntity)) //Ignore any local non-boids.
                    continue;

                if (otherBoidEntity == currentEntity) //Ignore self.
                    continue;

                const float distance = localAwareness->get_distances()[i];
                const float direction = localAwareness->get_directions()[i];
                const auto othersPosition = em->get_component<Component::Position>(otherBoidEntity).lock();

                //Calculate distance between currentEntity and boidEntity.
                //const auto othersPosition = em->get_component<Component::Position>(otherBoidEntity).lock();
                //const float xDistance = othersPosition->get_x() - curPosition->get_x();
                //const float yDistance = othersPosition->get_y() - curPosition->get_y();

                //const float distance = math2d::vector_magnitude(xDistance, yDistance); //Distance between the two boids (always positive or 0).
                //const float direction = math2d::cartesian_to_polar(xDistance, yDistance).second;

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
            auto newVelocity = math2d::polar_to_cartesian(curBoidComponent->maxVelocity, curPosition->get_orientation());
            curVelocity->set_dx(newVelocity.first);
            curVelocity->set_dy(newVelocity.second);
        }
    }

    //Increases velocity of entities in the forward direction.
    void Navigation::self_propel()
    {
        for (Entity currentEntity=0; currentEntity<em->component_list_size(); currentEntity++)
        {
            if (em->contains_component<Component::SelfPropelled>(currentEntity))
            {
                const auto propeller = em->get_component<Component::SelfPropelled>(currentEntity).lock();
                const auto position = em->get_component<Component::Position>(currentEntity).lock();
                auto velocity = em->get_component<Component::Velocity>(currentEntity).lock();

                //If not at maximum self-propelled velocity yet then accelerate forwards.
                auto unitVector = math2d::polar_to_cartesian(1.0, position->orientation);
                velocity->dx += unitVector.first * propeller->acceleration;
                velocity->dy += unitVector.second * propeller->acceleration;
            }
        }
    }

    //Scales down any velocity which exceeds the maximum.
    void Navigation::clip_to_max_velocity()
    {
        for (Entity currentEntity=0; currentEntity<em->component_list_size(); currentEntity++)
        {
            auto velocity = em->get_component<Component::Velocity>(currentEntity).lock();
            if (velocity->maxVelocity >= 0) //Negative values indicate no maximum velocity.
            {
                double magnitude = math2d::vector_magnitude(velocity->dx, velocity->dy);
                if (magnitude > velocity->maxVelocity) //If maximum velocity is being exceeded, scale to maximum velocity (without affecting orientation).
                {
                    velocity->dx = velocity->dx*(velocity->maxVelocity/magnitude);
                    velocity->dy = velocity->dy*(velocity->maxVelocity/magnitude);
                }
            }
        }
    }
}
