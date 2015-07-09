#pragma once

class StateManager;
class ResourceManager;

class GameState
{
protected:
    StateManager* stateManager;
    ResourceManager* resourceManager;
public:
    virtual void start()=0;
    virtual void stop()=0;
    virtual void update()=0;
    virtual void render()=0;
    virtual void pause()=0;
    virtual void unpause()=0;
    GameState(StateManager* _sm, ResourceManager* _rm) : stateManager(_sm), resourceManager(_rm) {  };
    virtual ~GameState() {  };
};
