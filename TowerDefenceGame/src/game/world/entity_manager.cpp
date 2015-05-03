#include "game/world/entity_manager.hpp"

//Removes an entity and all of it's components.
void EntityManager::destroy_entity(Entity _entityID)
{
    //Overwrite each component for entityID in each component list with nullptr.
    for (std::vector<std::shared_ptr<Component::BaseComponent>> &componentList : components)
        componentList[_entityID] = nullptr; //Smart pointers will release memory to pool automatically.

    //Set entity mask to all 0's.
    entityMasks[_entityID] = boost::dynamic_bitset<>(registry.get_size());

    //Add entityID to the deleted list.
    deletedEntities.push_back(_entityID);
}

//Creates an entity from xml.
//TODO: WARNING... Storing the returned Entity is NOT safe. Need to implement additional check (maybe a second number which is separate from the position, for entities). This could be used in conjunction with the position to check the entity is still value...
Entity EntityManager::create_entity(xml _config)
{
    Entity entityID;

    //First check if there's a gap from a deleted entity.
    if (!deletedEntities.empty())
    {
        entityID = deletedEntities.back(); //Find empty index to create entity in.
        deletedEntities.pop_back(); //Remove this index from the list of empty entities.
    }
    else //No gap, so create new entries.
    {
        entityMasks.emplace_back(registry.get_size());
        entityID = populate_empty_components();
    }

    //Iterate through the xml property tree and create required components.
    for (auto iter: _config.get_child("Entity")) //For each component in the xml property tree.
    {
        //Get component name
        std::string componentName = iter.first;

        //Convert name to ComponentID, which indicates which component list to use.
        unsigned int iComponentList;
        try
        {
            iComponentList = registry.get_ID(componentName);

            //Set the bit to true for this component, indicating that the entity is utilising this type of component.
            entityMasks[entityID][iComponentList] = true;

            //Create the component and places it in the correct list.
            components[iComponentList][entityID] = componentFactory.new_component(componentName);
        }
        catch (const std::runtime_error ex)
        {
            std::cerr << "Extracted unknown component name (" << componentName << ") from xml file in EntityManager::create_entity. Entity created ignoring component.\n"
                     << "Exception caught:\n" << ex.what() << "\n";
        }
    }

    //Return the tag of the newly created entity.
    return entityID;
}

//Pushes nullptrs to each component list.
Entity EntityManager::populate_empty_components()
{
    for (std::vector<std::shared_ptr<Component::BaseComponent> > &componentList : components)
        componentList.push_back(nullptr);

    return cListSize++;
}
