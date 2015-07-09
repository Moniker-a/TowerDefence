#pragma once
#include "game/states/game_state.hpp"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

class GameStateSplash : public GameState
{
private:
    bool paused = false;
    float alpha;
public:
    void start();
    void stop();
    void update();
    void render();
    void pause();
    void unpause();

    GameStateSplash(StateManager* _sm, ResourceManager* _rm) : GameState(_sm, _rm) {  }
};
