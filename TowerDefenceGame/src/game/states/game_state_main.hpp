#pragma once
#include "game/states/game_state.hpp"
#include "game/world/entity_manager.hpp"
#include "game/world/event_bus.hpp"
#include "game/world/system_manager.hpp"

class ResourceManager;

class GameStateMain : public GameState
{
private:
    //ResourceManager* resourceManager; //Access to resources.
    EntityManager entities; //Somewhere to store and manage Components.
    EventBus      eventBus; //Manages event passing and subscribing.
    SystemManager systems;  //Somewhere to store and use Systems.
public:
    void start();
    void stop();
    void update();
    void render();
    void pause();
    void unpause();

    GameStateMain(StateManager* _sm, ResourceManager* _rm) : GameState(_sm, _rm), entities(_rm), eventBus(), systems(&entities, &eventBus)
    {  }
};
