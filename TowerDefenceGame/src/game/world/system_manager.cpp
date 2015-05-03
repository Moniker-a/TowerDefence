#include "system_manager.hpp"
#include "game/world/entity_manager.hpp"
#include <algorithm>

//Updates each System in order of priority.
void SystemManager::update()
{
    //TODO: use iterators and/or for each loop?
    for (std::pair<const int, std::unique_ptr<System::BaseSystem> > &currentSystemPair : systemRegister) //For each system.
    {
        for (Entity iEntity = 0; iEntity < em->get_component_list_size(); iEntity++) //For each entity (and deleted entity).
        {
            boost::dynamic_bitset<> entityComponentsMask = em->get_entity_mask(iEntity); //Store entity's component mask (a bitset which indicates which components an entity has).
            boost::dynamic_bitset<> systemComponentsMask = currentSystemPair.second->get_system_mask(); //A System's component mask indicates which components it required.
            if (systemComponentsMask == (systemComponentsMask & entityComponentsMask)) //If the current entity contains all the required components for the current system to operate on.
            {
                //Call System update on the current entity.
                currentSystemPair.second->update(iEntity);
            }
        }
    }
}
