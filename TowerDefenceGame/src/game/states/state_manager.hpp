#pragma once
#include "game/states/game_state.hpp"
#include <stack>
#include <memory>


//Simple stacked game state manager.
class StateManager
{
private:
    std::stack<std::unique_ptr<GameState>> stateStack; //Stack of game states to allow easy transitioning to previous states/menus.
public:
    void push_state(std::unique_ptr<GameState> _newState); //Enters a new state, without discarding previous state.
    void pop_state(); //Properly destroys the active state.
    void switch_state(std::unique_ptr<GameState> _newState); //Enters a new state by replacing the previous state.
    GameState* get_current_state() { return stateStack.top().get(); } //Access to the current GameState.
};
