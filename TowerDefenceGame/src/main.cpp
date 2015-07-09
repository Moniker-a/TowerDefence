#include <iostream>
#include "game/world/world.hpp"
#include "game/game.hpp"

#include "game/world/event_bus.hpp"
#include "game/systems/collision_resolver.hpp"

int main()
{
    Game game;
    game.initialise();
    game.start();
    game.exit();

    //World::initalise();

    return 0;
}
