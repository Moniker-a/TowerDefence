#include "world.hpp"

#include <iostream> //Temp

#include "game/world/system_manager.hpp"
#include "game/world/entity_manager.hpp"
#include "game/world/event_bus.hpp"
#include "game/systems/collision_resolver.hpp"
#include "game/world/component_type_register.hpp"
#include "game/components/position.hpp"
#include "game/components/velocity.hpp"
#include "game/systems/movement.hpp"
#include "game/utility/xml.hpp"


namespace World
{
    namespace
    {
        EntityManager entities; //Somewhere to store and manage Components.
        SystemManager systems; //Somewhere to store and use Systems.
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
        ///////////////////////////

        //Lets register some Components.
        entities.register_component<Component::Position>();
        entities.register_component<Component::Velocity>();
        entities.complete_registration(); //This signals end of registration.
                                            //We cannot register any components after complete_registration has been called because this is when
                                            //the Component lists are created (see ComponentManager).

        //Lets specify some entities!
        xml entityconfig1; //These will eventually be loaded from XML
        entityconfig1.put("Entity","");
        entityconfig1.put("Entity.Position","");
        entityconfig1.put("Entity.Velocity", "");

        xml entityconfig2; //These will eventually be loaded from XML
        entityconfig2.put("Entity","");
        entityconfig2.put("Entity.Velocity", "");

        xml entityconfig3; //These will eventually be loaded from XML
        entityconfig3.put("Entity","");
        entityconfig3.put("Entity.Position","");
        entityconfig3.put("Entity.Velocity", "");

        //Create the entities we just specified.
        entities.create_entity(entityconfig1);
        entities.create_entity(entityconfig2);
        entities.create_entity(entityconfig3);

        //Lets register a system.
        systems.register_system<System::Movement>(1); //Priority of 1.

        //Update all the systems and entities.
        systems.update(entities);

        //Remove the first entity.
        entities.destroy_entity(0);

        //Update all the systems and entities again.
        systems.update(entities);

        //Example of event system
        System::CollisionResolver cr; //Create a system which is capable of handling events.
        System::CollisionResolver cr2; //Create another system which is capable of handling events.
        EventBus eb;
        eb.addSubscriber<CollisionEvent>(cr); //Tell the event bus to notify System 'cr' whenever a CollisionEvent occurs.
        eb.addSubscriber<CollisionEvent>(cr2); //Tell the event bus to notify System 'cr2' whenever a CollisionEvent occurs.
        CollisionEvent ce(1,2); //Create a collision event (note: other systems would usually do this).
        eb.sendEvent<CollisionEvent>(ce); //Sent event to the bus - which will automatically notify any Systems which have subscribed this this event.
    }
}
