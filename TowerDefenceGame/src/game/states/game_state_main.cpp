#include "game_state_main.hpp"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "game/states/state_manager.hpp"
#include "game/world/resource_manager.hpp"
#include "game/resources/wrapped_resource.hpp"

#include "game/world/world.hpp"
#include "game/events/render_event.hpp"

#include <iostream> //temp

void GameStateMain::start()
{
    std::cout << "Starting state: GameStateMain.\n";

    resourceManager->add_resource<WrappedBitmap, const char*>("eiffel tower", "resources/eiffel.png");
    resourceManager->add_resource<WrappedBitmap, const char*>("sun", "resources/sun.png");
    resourceManager->add_resource<WrappedBitmap, const char*>("zombie", "resources/zombie.png");
    //resourceManager->add_resource<WrappedXML, const char*>("test entity", "resources/xml/test_entity.xml");
    //resourceManager->add_resource<WrappedXML, const char*>("test entity2", "resources/xml/test_entity2.xml");
    resourceManager->add_resource<WrappedXML, const char*>("attractor entity", "resources/xml/attractor_entity.xml");
    resourceManager->add_resource<WrappedXML, const char*>("attractee entity", "resources/xml/attractee_entity.xml");
    resourceManager->add_resource<WrappedXML, const char*>("attractee entity2", "resources/xml/attractee_entity2.xml");
    resourceManager->add_resource<WrappedXML, const char*>("attractee entity3", "resources/xml/attractee_entity3.xml");

    World::initalise(resourceManager);
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
}

void GameStateMain::update()
{
    World::update();
}

void GameStateMain::render()
{
    al_clear_to_color(al_map_rgb(200, 150, 200));

    World::render();
}

void GameStateMain::pause()
{
    std::cout << "Pausing state: GameStateMain.\n";
}

void GameStateMain::unpause()
{
    std::cout << "Resuming state: GameStateMain.\n";
}
