#include "game_state_main.hpp"

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "game/components/attractee.hpp"
#include "game/components/attractor.hpp"
#include "game/components/boid.hpp"
#include "game/components/bounding_box.hpp"
#include "game/components/debug_info.hpp"
#include "game/components/entity_spawner.hpp"
#include "game/components/locally_aware.hpp"
#include "game/components/position.hpp"
#include "game/components/renderable.hpp"
#include "game/components/self_propelled.hpp"
#include "game/components/velocity.hpp"

#include "game/systems/collision_resolver.hpp"
#include "game/systems/debug.hpp"
#include "game/systems/entity_creation.hpp"
#include "game/systems/movement.hpp"
#include "game/systems/navigation.hpp"
#include "game/systems/perception.hpp"
#include "game/systems/render.hpp"
#include "game/systems/wrap_detection.hpp"

#include "game/events/render_event.hpp"
#include "game/events/debug_info_display.hpp"

#include "game/states/state_manager.hpp"
#include "game/world/resource_manager.hpp"
#include "game/resources/wrapped_resource.hpp"

#include <iostream> //temp

void GameStateMain::start()
{
    resourceManager->add_resource<WrappedBitmap, const char*>("eiffel tower", "resources/eiffel.png");
    resourceManager->add_resource<WrappedBitmap, const char*>("sun", "resources/sun.png");
    resourceManager->add_resource<WrappedBitmap, const char*>("zombie", "resources/zombie.png");
    resourceManager->add_resource<WrappedBitmap, const char*>("boid red", "resources/boid_red.png");
    resourceManager->add_resource<WrappedBitmap, const char*>("boid", "resources/boid.png");
    resourceManager->add_resource<WrappedXML, const char*>("test entity", "resources/xml/test_entity.xml");
    resourceManager->add_resource<WrappedXML, const char*>("test entity2", "resources/xml/test_entity2.xml");
    resourceManager->add_resource<WrappedXML, const char*>("attractor entity", "resources/xml/attractor_entity.xml");
    resourceManager->add_resource<WrappedXML, const char*>("attractee entity", "resources/xml/attractee_entity.xml");
    resourceManager->add_resource<WrappedXML, const char*>("attractee entity2", "resources/xml/attractee_entity2.xml");
    resourceManager->add_resource<WrappedXML, const char*>("attractee entity3", "resources/xml/attractee_entity3.xml");
    resourceManager->add_resource<WrappedXML, const char*>("boid entity", "resources/xml/boid_entity.xml");
    resourceManager->add_resource<WrappedXML, const char*>("boid entity2", "resources/xml/boid_entity2.xml");

    //Old World initialiser...
    entities.register_component<Component::Position>(); //Component which stores an entities position.
    entities.register_component<Component::Velocity>(); //Stores an entities velocity.
    entities.register_component<Component::BoundingBox>(); //Stores information on the region for which entity position is wrapped to.
    entities.register_component<Component::DebugInfo>();
    entities.register_component<Component::Renderable>();
    entities.register_component<Component::Attractor>();
    entities.register_component<Component::Attractee>();
    entities.register_component<Component::Boid>();
    entities.register_component<Component::EntitySpawner>();
    entities.register_component<Component::SelfPropelled>();
    entities.register_component<Component::LocallyAware>();

    entities.complete_registration(); //This signals end of registration.
                                      //We cannot register any components after complete_registration has been called because this is when
                                      //the Component lists are created (see ComponentManager).

    //Next we need to register our systems. These contain the logic which change the state (components) of game entities.
    systems.register_system<System::Perception>(0, "perception");
    systems.register_system<System::Navigation>(1, "navigation");
    systems.register_system<System::Movement>(2, "movement"); //Priority of 1 (determines the order in which systems are updated).
                                                              //You can also optionally name a system when registering it, this allows it to be accessed directly using SystemManager::get_system(name);
    systems.register_system<System::WrapDetection>(3, "wrap detection"); //Priority of 2, because we want to check to see if a position needs to be wrapped *after* moving entities.
    systems.register_system<System::Render, ResourceManager*>(10, resourceManager, "render");
    systems.register_system<System::EntityCreation, ResourceManager*>(10, resourceManager, "entity creation");
    systems.register_system<System::Debug, ResourceManager*>(20, resourceManager, "debug");

    //Next we need to specify which Systems listen for which events.
    //In this case we need the Movement system to listen for PositionWrapEvent events.
    //TODO: These could be done inside System constructors, automating the process, though makes the systems / event system less reusable in future projects?
    eventBus.add_subscriber<Event::MoveEntityEvent>(systems.get_system<System::Movement>("movement"));
    eventBus.add_subscriber<Event::RenderEvent>(systems.get_system<System::Render>("render"));
    eventBus.add_subscriber<Event::TriggerEntityCreationEvent>(systems.get_system<System::EntityCreation>("entity creation"));
    eventBus.add_subscriber<Event::DebugInfoDisplay>(systems.get_system<System::EntityCreation>("debug"));

    //entities.create_entity(resourceManager->get_resource<WrappedXML>("attractor entity"));
    //entities.create_entity(resourceManager->get_resource<WrappedXML>("attractee entity"));
    //entities.create_entity(resourceManager->get_resource<WrappedXML>("attractee entity2"));
    //entities.create_entity(resourceManager->get_resource<WrappedXML>("attractee entity3"));

    //Create a tonne of entities.
    //entities.create_entity("boid entity", "Entity.Position.<xmlattr>.orientation", 3.1415);
    //entities.create_entity(resourceManager->get_resource<WrappedXML>("boid entity"), "Entity.Position.<xmlattr>.orientation", 3.1415);
    entities.create_entity(resourceManager->get_resource<WrappedXML>("test entity"));
    entities.create_entity(resourceManager->get_resource<WrappedXML>("test entity2"));
    //WrappedXML boidEntityTemplate(resourceManager->get_resource<WrappedXML>("boid entity")); //Make a copy of the boid entity specification so we can alter it.
    //entities.create_entity(boidEntityTemplate);
    /*for (unsigned int i=0; i<10; i++)
    {
        for (unsigned int j=0; j<8; j++)
        {
            boidEntityTemplate.set_attribute("Entity.Position.<xmlattr>.x", i*90);
            boidEntityTemplate.set_attribute("Entity.Position.<xmlattr>.y", j*110);
            boidEntityTemplate.set_attribute("Entity.Position.<xmlattr>.orientation", ((float)(i*i)) / (float)(j+1));
            entities.create_entity(boidEntityTemplate);
        }
    }*/
}

void GameStateMain::stop()
{
    std::cout << "Stopping state: GameStateMain.\n";
    resourceManager->erase_resource("eiffel tower");
    resourceManager->erase_resource("sun");
    resourceManager->erase_resource("zombie");
    resourceManager->erase_resource("attractor entity");
    resourceManager->erase_resource("attractee entity");
    resourceManager->erase_resource("attractee entity2");
    resourceManager->erase_resource("attractee entity3");

    resourceManager->erase_resource("boid");
    resourceManager->erase_resource("boid2");
    resourceManager->erase_resource("boid entity");
    resourceManager->erase_resource("boid entity2");
}

void GameStateMain::update()
{
    systems.update();
}

void GameStateMain::render()
{
    al_clear_to_color(al_map_rgb(200, 150, 200)); //Shouldn't Systems::Render be doing this?
    eventBus.send_event<Event::RenderEvent>(Event::RenderEvent());
}

void GameStateMain::pause()
{
    std::cout << "Pausing state: GameStateMain.\n";
}

void GameStateMain::unpause()
{
    std::cout << "Resuming state: GameStateMain.\n";
}
