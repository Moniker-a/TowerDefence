#pragma once
#include "game/states/state_manager.hpp"
#include <allegro5/allegro.h>
#include "game/world/resource_manager.hpp"


//Initialised Allegro and managed high-level classes (e.g. StateManager, ResourceManager, World).
//Loads the initial game state.
class Game //Consider implementing 'SingleInstance' from here http://stackoverflow.com/questions/3926530/a-singleton-that-is-not-globally-accessible
{
private:
    ResourceManager resourceManager;
    StateManager stateManager;

    ALLEGRO_DISPLAY* pdisplay = NULL;
    ALLEGRO_TIMER* prenderTimer = NULL;
    ALLEGRO_EVENT_QUEUE* pqueue = NULL;

    bool running = false;
    bool gameExitedCorrectly = false;

public:
    //Initialises application resources which are necessary for all aspects of the game to run.
    bool initialise();

    //Start the main loop running with GameState::SPLASH
    void start();

    //Pauses the game (does not destroy any resources).
    void pause();

    //Exits the game destroying all resources.
    void exit();

    //Checks if game exited correctly and flags an error if exit() hasn't been called.
    //This is currently a work-around to ensure objects which use Allegro are shutdown before Allegro is.
    ~Game();
};
