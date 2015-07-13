#pragma once

#include <vector>
#include <memory>
#include <boost/dynamic_bitset.hpp>
#include <boost/noncopyable.hpp>
#include "game/world/world.hpp"
#include "game/world/component_type_register.hpp"
#include "game/world/component_factory.hpp"
#include "game/utility/xml.hpp"
#include "game/world/entity.hpp"

//EntityManager handles the component level storage of game objects. At its core it consists of a list of lists of components. Each list comtains a different type of component.
//When an entity is creates a new element is created on each list. The entity ID (Entity) is used to index into this list and retrieve components.
//Systems should not interact directly with the EntityManager but must do so via an EntityAccessLayer which ensures they only access components from entities they're supposed to be accessing.
class EntityManager : boost::noncopyable
{
  private:
    std::size_t cListSize; //Stores the size of the component lists. Always equal to the number of entities.

    std::vector< std::vector< std::shared_ptr<Component::BaseComponent>>> components; //Vector of vectors of components - holds all components. Vector of component lists.
    std::vector<boost::dynamic_bitset<>> entityMasks; //Vector of bitsets, containing the bitmasks for each entity. This indicates which entities utilise which components.
    ComponentTypeRegister registry; //A class to convert a component name into an index value.
    ComponentFactory componentFactory; //This facilitates the creation of components.
    bool initalised; //When set to true, no more components can be registered and the component lists are created.
    std::list<Entity> deletedEntities; //Contains the EntityIDs (indices) of deleted entities.

  public:
    EntityManager() : cListSize(0), initalised(false) {  } //Default constructor.

    void destroy_entity(Entity _entityID); //Removes an entity and all of it's components.

    //TODO: WARNING... Storing the returned Entity is NOT safe. Need to implement additional check (maybe a second number which is separate from the position, for entities). This could be used in conjunction with the position to check the entity is still value...
    Entity create_entity(Xml _config); //Creates an entity from xml. Mostly a wrapper for EntityManager.

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
        throw std::runtime_error("Error: EntityManager::get_component<T>(Entity _entity) - Attempted to retrieve uninitialised component");

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


