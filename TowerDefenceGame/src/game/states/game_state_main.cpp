#include "game_state_main.hpp"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "game/states/state_manager.hpp"

#include "game/world/world.hpp"
#include "game/events/render_event.hpp"

#include <iostream> //temp

void GameStateMain::start()
{
    std::cout << "Starting state: GameStateMain.\n";
    World::initalise();
}

void GameStateMain::stop()
{
    std::cout << "Stopping state: GameStateMain.\n";
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
