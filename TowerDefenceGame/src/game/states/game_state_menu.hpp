#pragma once
#include "game/states/game_state.hpp"

class GameStateMenu : public GameState
{
private:
    int blue = 0;
public:
    void start();
    void stop();
    void update();
    void render();
    void pause();
    void unpause();

    GameStateMenu(StateManager* _sm, ResourceManager* _rm) : GameState(_sm, _rm) {  }
};

