#include "state_manager.hpp"

void StateManager::push_state(std::unique_ptr<GameState> _newState)
{
    if (!stateStack.empty()) //If this isn't the first state, then obscure the previous state.
        stateStack.top()->pause();

    //Add and start the new state.
    stateStack.push(std::move(_newState));
    stateStack.top()->start();
}

//Removes from stack and properly destroys the active state.
void StateManager::pop_state()
{
    //Stop the current state and remove it.
    stateStack.top()->stop();
    stateStack.pop();

    if (!stateStack.empty()) //If this wasn't the last state reveal the new top state.
        stateStack.top()->unpause();
}

//Replace the current state with another.
void StateManager::switch_state(std::unique_ptr<GameState> _newState)
{
    if (stateStack.empty())
        push_state(std::move(_newState));
    else
    {
        pop_state(); //Automatically stop()s the top() state.
        push_state(std::move(_newState));
    }
}
