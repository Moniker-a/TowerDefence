#include "game_state_splash.hpp"
#include <stdexcept>
#include <iostream>
#include "game/states/state_manager.hpp"
#include "game/states/game_state_menu.hpp"
#include "game/world/resource_manager.hpp"
#include "../resources/wrapped_resource.hpp"

void GameStateSplash::start()
{
    std::cout << "Starting state: GameStateSplash.\n";

    alpha = 0.0;

    resourceManager->add_resource<WrappedBitmap, const char*>("splash screen image", "resources/temp_splash.jpg");
}

void GameStateSplash::stop()
{
    std::cout << "Stopping state: GameStateSplash.\n";
    resourceManager->erase_resource("splash screen image");
}

void GameStateSplash::update()
{
    if (!paused)
    {
        //Update alpha
        alpha += 0.002;

        if (alpha >= 0.8)
        {
            stateManager->switch_state(std::unique_ptr<GameState>(new GameStateMenu(stateManager, resourceManager)));
        }
    }
}

void GameStateSplash::render()
{
    if (!paused)
        al_draw_tinted_bitmap(resourceManager->get_resource<WrappedBitmap>("splash screen image"), al_map_rgba_f(1, 1, 1, alpha), 0, 0, 0);
}

void GameStateSplash::pause()
{
    std::cout << "Pausing state: GameStateSplash.\n";
    paused = true;
}

void GameStateSplash::unpause()
{
    std::cout << "Resuming state: GameStateSplash.\n";
    paused = false;
}
