#include "game/world/entity_manager.hpp"

//Removes an entity and all of it's components.
void EntityManager::destroy_entity(Entity _entityID)
{
    //Overwrite each component for entityID in each component list with nullptr.
    for (std::vector<std::shared_ptr<Component::BaseComponent>> &componentList : components)
        componentList[_entityID] = nullptr; //Smart pointers will release memory to pool automatically.

    //Set entity mask to all 0's.
    entityMasks[_entityID] = boost::dynamic_bitset<>(registry.size());

    //Added entityID to the deleted list.
    deletedEntities.push_back(_entityID);
}

//Pushes nullptrs to each component list.
Entity EntityManager::populate_empty_components()
{
    for (std::vector<std::shared_ptr<Component::BaseComponent> > &componentList : components)
        componentList.push_back(nullptr);

    return cListSize++;
}


//Returns true is the supplied mask is a subset of the entity's mask.
bool EntityManager::match_mask(const Entity _entity, const boost::dynamic_bitset<> &_maskToMatch) const
{
    if ((get_entity_mask(_entity) & _maskToMatch) == _maskToMatch)
        return true;
    else
        return false;
}
