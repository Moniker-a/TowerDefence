#pragma once
#include "game/states/game_state.hpp"

class GameStateMain : public GameState
{
private:
    //
public:
    void start();
    void stop();
    void update();
    void render();
    void pause();
    void unpause();

    GameStateMain(StateManager* _sm, ResourceManager* _rm) : GameState(_sm, _rm) {  }
};
