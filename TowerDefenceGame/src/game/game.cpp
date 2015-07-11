#include "game.hpp"
#include <stdexcept>
#include <memory>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include "game/states/game_state_splash.hpp"

//Temp
#include "game/states/game_state_main.hpp"
#include "game/resources/wrapped_resource.hpp"
#include <allegro_image.h>


//Initialises application resources which are necessary for all aspects of the game to run.
bool Game::initialise()
{
    //Initialise Allegro.
    if (!al_init())
        throw std::runtime_error("Couldn't initialise allegro.");

    //Initialise specific Allegro modules.
    al_init_image_addon();
    al_install_mouse();
    al_install_keyboard();
    al_init_primitives_addon();

    //Initialise Allegro resources.
    prenderTimer = al_create_timer(1.0/60);
    pqueue = al_create_event_queue();
    pdisplay = al_create_display(1023, 575);

    //Register event sources.
    al_register_event_source(pqueue, al_get_display_event_source(pdisplay));
    al_register_event_source(pqueue, al_get_timer_event_source(prenderTimer));

    ///Test stuff here?

    //al_clear_to_color(al_map_rgb(100,100,100));
    //al_draw_bitmap(bit, 100, 100, 0);
    //al_flip_display();
    //al_rest(3);

    //Load state transcending resources.
    resourceManager.add_resource<WrappedBitmap, const char*>("default bitmap", "resources/default_bitmap.png");

    //Set the initial game state.
    //stateManager.push_state(std::unique_ptr<GameState>(new GameStateMain(&stateManager, &resourceManager)));
    stateManager.push_state(std::unique_ptr<GameState>(new GameStateSplash(&stateManager, &resourceManager))); //For quick testing!

    return true;
}

//Start the main loop running with GameState::SPLASH
void Game::start()
{
    running = true;
    bool render = false;
    al_start_timer(prenderTimer);

    while (running)
    {
        //Update World?

        //Wait for an event.
        ALLEGRO_EVENT event;
        al_wait_for_event(pqueue, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            exit();

        if (event.type == ALLEGRO_EVENT_DISPLAY_SWITCH_OUT)
            pause();

        if (event.type == ALLEGRO_EVENT_TIMER)
        {
            render = true;
        }

        //Update gamestate.
        stateManager.get_current_state()->update();

        if (render == true)
        {
            render = false;

            stateManager.get_current_state()->render();
            al_flip_display();
        }
    }
}

//Pauses the game (does not destroy any resources).
void Game::pause()
{
    al_stop_timer(prenderTimer);
    while (true)
    {
        ALLEGRO_EVENT event;
        al_wait_for_event(pqueue, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_SWITCH_IN)
        {
            al_start_timer(prenderTimer);
            break; //Exit the pause lock...
        }
    }
}

//Exits the game destroying all resources.
void Game::exit()
{
    running = false;

    //Clear the ResourceManager - this is important to ensure Allegro objects are properly destroyed before shutting down Allegro itself.
    resourceManager.clear();

    //Destroy stuff which doesn't automatically destroy itself...
    al_destroy_timer(prenderTimer);
    al_destroy_event_queue(pqueue);
    al_destroy_display(pdisplay);

    //Shutdown Allegro addon modules.
    al_shutdown_primitives_addon();
    //al_shutdown_images_addon();

    gameExitedCorrectly = true;
}

Game::~Game()
{
    if (gameExitedCorrectly == false)
        std::cout << "WARNING: Game::exit() was not called. Allegro objects may be destroyed after Allegro has shut down causing segfaults or undefined behaviour!" << std::endl;
}
