#include "game_state_menu.hpp"

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "game/states/state_manager.hpp"
#include "game/states/game_state_main.hpp"

#include <iostream> //temp

void GameStateMenu::start()
{
    std::cout << "Starting state: GameStateMenu.\n";
}

void GameStateMenu::stop()
{
    std::cout << "Stopping state: GameStateMenu.\n";
}

void GameStateMenu::update()
{
    //Update.
    blue += 1;

    if (blue >= 256)
        stateManager->switch_state(std::unique_ptr<GameState>(new GameStateMain(stateManager, resourceManager)));
}

void GameStateMenu::render()
{
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_filled_rounded_rectangle(400, 100, 600, 175, 25, 10, al_map_rgb(200, 200, 0));
    al_draw_filled_rounded_rectangle(400, 200, 600, 275, 25, 10, al_map_rgb(200, 200, 0));
    al_draw_filled_rounded_rectangle(400, 300, 600, 375, 25, 10, al_map_rgb(200, 200, blue));
}

void GameStateMenu::pause()
{
    std::cout << "Pausing state: GameStateMenu.\n";
}

void GameStateMenu::unpause()
{
    std::cout << "Resuming state: GameStateMenu.\n";
}
