#include "world.hpp"

#include <iostream> //Temp

#include "game/world/system_manager.hpp"
#include "game/world/entity_manager.hpp"
#include "game/world/event_bus.hpp"
#include "game/systems/collision_resolver.hpp"
#include "game/world/component_type_register.hpp"
#include "game/components/position.hpp"
#include "game/components/velocity.hpp"
#include "game/components/bounding_box.hpp"
#include "game/systems/movement.hpp"
#include "game/systems/wrap_detection.hpp"
#include "game/events/move_entity_event.hpp"
#include "game/utility/xml.hpp"

namespace World
{
    namespace
    {
        EntityManager entities; //Somewhere to store and manage Components.
        EventBus      eventBus; //Manages event passing and subscribing.
        SystemManager systems(&entities, &eventBus);  //Somewhere to store and use Systems.
    }

    //Get a const copy of registry.
    //Currently used by Systems so they can automatically create their component masks regardless of order in which Components are registered with the ComponentManager.
    const ComponentTypeRegister& get_registry()
    {
        return entities.get_component_type_register();
    }

    //Initialise World (currently just contains usage demonstration code).
    void initalise()
    {
        ///////////////////////////
        //  Demonstration code:  //
        ///////////////////////////////////////////////////////////////////////////////////////////
        // Three entities are created, each with different properties.
        // The first entity has a position but no velocity (i.e. it is stationary).
        // The second entity has a position and velocity (it is moving).
        // The third entity has position, velocity and wraps around the camera's field of view.
        //
        //

        //Lets register some Components. These are the building blocks of game objects (entities). They contain all the data which describe the current state of an entity.
        entities.register_component<Component::Position>(); //Component which stores an entities position.
        entities.register_component<Component::Velocity>(); //Stores an entities velocity.
        entities.register_component<Component::BoundingBox>(); //Stores information on the region for which entity position is wrapped to.
        entities.complete_registration(); //This signals end of registration.
                                          //We cannot register any components after complete_registration has been called because this is when
                                          //the Component lists are created (see ComponentManager).

        //Next we need to register our systems. These contain the logic which change the state (components) of game entities.
        systems.register_system<System::Movement>(1, "movement"); //Priority of 1 (determines the order in which systems are updated).
                                                                  //You can also optionally name a system when registering it, this allows it to be accessed directly using SystemManager::get_system(name);
        systems.register_system<System::WrapDetection>(2, "wrap detection"); //Priority of 2, because we want to check to see if a position needs to be wrapped *after* moving entities.

        //Next we need to specify which Systems listen for which events.
        //In this case we need the Movement system to listen for PositionWrapEvent events.
        eventBus.add_subscriber<MoveEntityEvent>(systems.get_system<System::Movement>("movement"));

        //Lets specify our entities by creating three XML files. These would usually exist already.
        xml entityConfig0;
        entityConfig0.put("Entity","");
        entityConfig0.put("Entity.Position",""); //We use the string name 'Position' to specify which type of component to be created.
                                                 //This string is matched to the overridden Component::get_name() function.
        xml entityConfig1;
        entityConfig1.put("Entity","");
        entityConfig1.put("Entity.Position","");
        entityConfig1.put("Entity.Velocity", "");

        xml entityConfig2;
        entityConfig2.put("Entity","");
        entityConfig2.put("Entity.Position","");
        entityConfig2.put("Entity.Velocity", "");
        entityConfig2.put("Entity.BoundingBox", "");

        //Create the entities we just specified (these would usually be loaded straight from XML).
        entities.create_entity(entityConfig0);
        entities.create_entity(entityConfig1);
        entities.create_entity(entityConfig2);

        //Update all the systems and entities a few times.
        for (unsigned int i=0; i<15; i++)
        {
            std::cout << "\nUpdate " << i << "\n";
            systems.update();
        }

        //Remove whichever entity is at position 0.
        std::cout << "\n\nDestroying entity.";
        entities.destroy_entity(0);

        std::cout << "\n\nEnd of example.";
    }
}
