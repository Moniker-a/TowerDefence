#pragma once

#include <vector>
#include <memory>
#include <boost/dynamic_bitset.hpp>
#include <boost/noncopyable.hpp>
#include "game/world/component_type_register.hpp"
#include "game/world/component_factory.hpp"
#include "game/world/resource_manager.hpp"
#include "game/resources/wrapped_xml.hpp"
#include "game/utility/xml.hpp"
#include "game/world/entity.hpp"

//EntityManager handles the component level storage of game objects. At its core it consists of a list of lists of components. Each list comtains a different type of component.
//When an entity is creates a new element is created on each list. The entity ID (Entity) is used to index into this list and retrieve components.
//Systems should not interact directly with the EntityManager but must do so via an EntityAccessLayer which ensures they only access components from entities they're supposed to be accessing.
class EntityManager : boost::noncopyable
{
private:
    ResourceManager* resourceManager;
    std::size_t cListSize; //Stores the size of the component lists. Always equal to the number of entities.

    std::vector< std::vector< std::shared_ptr<Component::BaseComponent>>> components; //Vector of vectors of components - holds all components. Vector of component lists.
    std::vector<boost::dynamic_bitset<>> entityMasks; //Vector of bitsets, containing the bitmasks for each entity. This indicates which entities utilise which components.
    ComponentTypeRegister registry; //A class to convert a component name into an index value.
    ComponentFactory componentFactory; //This facilitates the creation of components.
    bool initalised; //When set to true, no more components can be registered and the component lists are created.
    std::list<Entity> deletedEntities; //Contains the EntityIDs (indices) of deleted entities.

    //Used to customise base specification of xml attributes recursively (base case - makes no changes).
    void recursivelyUpdateAttributes(Xml& _config) { /*Empty*/ };
    //Used to customise base specification of xml attributes recursively.
    template <typename ATTR_PATH, typename VALUE, typename ...OptionalOverriders>
    void recursivelyUpdateAttributes(Xml& _config, ATTR_PATH _path, VALUE _value, OptionalOverriders... optionalOverriders);

public:
    EntityManager(ResourceManager* _rm) : resourceManager(_rm), cListSize(0), initalised(false) {  } //Default constructor.

    void destroy_entity(Entity _entityID); //Removes an entity and all of it's components.

    //TODO: WARNING... Storing the returned Entity is NOT safe. Need to implement additional check (maybe a second number which is separate from the position, for entities). This could be used in conjunction with the position to check the entity is still value...
    template <typename ...OptionalOverriders>
    Entity create_entity(const Xml& _config, OptionalOverriders... optionalOverriders); //Creates an entity from xml. Mostly a wrapper for EntityManager.

    //Overloaded create_entity just retrieves xml resource for the user.
    template <typename ... OptionalOverriders>
    Entity create_entity(const char* _config, OptionalOverriders... optionalOverriders);

    const boost::dynamic_bitset<>& get_entity_mask(Entity _entityID) const { return entityMasks.at(_entityID); } //Returns the entity component mask of an entity.

    std::size_t component_list_size() const { return cListSize; } //Returns number of components in each list.
    std::size_t num_component_types() const { return registry.size(); }
    bool match_mask(const Entity _entity, const boost::dynamic_bitset<>& _maskToMatch) const; //Returns true is the supplied mask is a subset of the entity's mask.

    template <typename T>
    void register_component(); //Registers a component type with the world.

    void complete_registration() { initalised = true; } //Close the component manager to registering new component types.
    bool is_registration_complete() const { return initalised; } //Returns true if component registration is closed (i.e. complete_registration() has been called).
    Entity populate_empty_components(); //Adds nullptr's to each component list, returns index of the newly added elements (i.e. index to empty entity).
    void initialise_component(Entity _iEntity, unsigned int _iComponentList, std::string _component_name); ////Initialises a component (when an entity is created it is given nullptrs in place of initialised components).
    //void erase_components(Entity _iEntity); //Removed the components from each component_list at index _iEntity.
    //const ComponentTypeRegister& get_component_type_register() const { return registry; }

    template <typename ComponentType>
    const ComponentID get_component_id() { return registry.get_ID<ComponentType>(); }

    template <typename ComponentType>
    bool contains_component(const Entity _entity) { return entityMasks.at(_entity)[get_component_id<ComponentType>()]; }

    template <class TComponentType>
    std::weak_ptr<TComponentType> get_component(const Entity _entity) const; //Returns a pointer to an existing component (who's type is specified by TComponentType) from the entity specified by _entity.
};

//Returns a pointer to an existing component (who's type is specified by TComponentType) from the entity specified by _entity.
template <class TComponentType>
std::weak_ptr<TComponentType> EntityManager::get_component(const Entity _entity) const
{
    // checks if a pointer was passed
    if (std::is_pointer<TComponentType>::value)
        throw std::runtime_error("Error: EntityManager::get_component<TComponentType>(Entity _entity) - TComponentType cannot be a pointer");

    // confirms that T is a) a base of Component or b) Component (default)
    if (!std::is_base_of<Component::BaseComponent,TComponentType>::value || std::is_same<Component::BaseComponent,TComponentType>::value)
        throw std::runtime_error("Error: EntityManager::get_component<TComponentType>(Entity _entity) - TComponentType must be base of Component");

    // checks that the component has been registered
    if (!registry.exists(TComponentType::get_name()))
        throw std::runtime_error("Error: EntityManager::get_component<TComponentType>(Entity _entity) - TComponentType must be a registered component");

    // converts the component name into an int for indexing into the component array
    unsigned int component_index = registry.get_ID(TComponentType::get_name());

    // if the bitmask for that particular entity indicates it does not have the component, return nullptr.
    if (components[component_index][_entity] == nullptr)
        throw std::runtime_error("Error: EntityManager::get_component<T>(Entity _entity) - Attempted to retrieve uninitialised component: "+TComponentType::get_name());

    // else return a pointer to the correct component
    return std::weak_ptr<TComponentType>(std::static_pointer_cast<TComponentType>(components[component_index][_entity]));
}

//Registers a component type and creates a new component list to hold components of this type.
template <class TComponentType>
void EntityManager::register_component()
{
    // checks if a pointer was passed
    if (std::is_pointer<TComponentType>::value)
        throw std::runtime_error("Error: EntityManager::register_component<TComponentType>() - TComponentType cannot be a pointer.\n");

    // confirms that T is a base of Component
    if (!std::is_base_of<Component::BaseComponent,TComponentType>::value)
        throw std::runtime_error("Error: EntityManager::register_component<TComponentType>() - TComponentType must be base of Component.\n");

    std::string componentName = TComponentType::get_name();

    //Check it's not a duplicate.
    if (registry.exists(componentName))
        throw std::runtime_error("Error: EntityManager::register_component<TComponentType>() - Cannot register a component twice ("+componentName+").\n");

    // adds component to the registry
    registry.add(componentName);

    //Tell the componentFactory about the component type.
    componentFactory.register_component<TComponentType>();

    //Create a new vector for storing the new type.
    components.push_back(std::vector< std::shared_ptr<Component::BaseComponent> >());
}

//Creates an entity from xml.
//TODO: WARNING... Storing the returned Entity is NOT safe. Need to implement additional check (maybe a second number which is separate from the position, for entities). This could be used in conjunction with the position to check the entity is still value...
template <typename ... OptionalOverriders>
Entity EntityManager::create_entity(const Xml& _config, OptionalOverriders... optionalOverriders)
{
    //Check any supplied attribute overriders are an even number (since they need to correspond to a path-value pairs).
    if (sizeof...(optionalOverriders) % 2 != 0)
        throw std::runtime_error("EntityManager::create_entity: OptionalOverriders must be supplied as ComponentName-Tuple pairs.");

    Xml newConfig(_config); //Copy the xml configuration before making any changes.
    recursivelyUpdateAttributes(newConfig, optionalOverriders...); //recursively peels off pairs of optionalOverriders, treating the first as the attribute path and the second as the parameter to write.

    Entity entityID;

    //First check if there's a gap from a deleted entity.
    if (!deletedEntities.empty())
    {
        entityID = deletedEntities.back(); //Find empty index to create entity in.
        deletedEntities.pop_back(); //Remove this index from the list of empty entities.
    }
    else //No gap, so create new entries.
    {
        entityMasks.emplace_back(registry.size());
        entityID = populate_empty_components();
    }

    //Iterate through the xml property tree and create required components.
    for (auto iter : newConfig.get_child("Entity")) //For each component in the xml property tree.
    {
        //Get component name
        std::string componentName = iter.first;

        //Convert name to ComponentID, which indicates which component list to use.
        unsigned int iComponentID;
        try
        {
            iComponentID = registry.get_ID(componentName);

            //Set the bit to true for this component, indicating that the entity is utilising this type of component.
            entityMasks[entityID][iComponentID] = true;

            //Create the component using the xml specification and place it in the correct list.
            components[iComponentID][entityID] = componentFactory.new_component(componentName, newConfig);
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

//Overloaded create_entity just retrieves xml resource for the user.
template <typename ... OptionalOverriders>
Entity EntityManager::create_entity(const char* _config, OptionalOverriders... optionalOverriders)
{
    return create_entity(resourceManager->get_resource<WrappedXML>(_config), optionalOverriders...);
}

//Private function used to customise base specification of xml attributes recursively.
template <typename ATTR_PATH, typename VALUE, typename ...OptionalOverriders>
void EntityManager::recursivelyUpdateAttributes(Xml& _config, ATTR_PATH _path, VALUE _value, OptionalOverriders... _optionalOverriders)
{
    _config.put(_path, _value);
    recursivelyUpdateAttributes(_config, _optionalOverriders...);
}

