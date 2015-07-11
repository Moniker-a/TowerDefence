#pragma once
#include <string>
#include <memory>
#include <unordered_map>

#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>

#include "game/components/component.hpp"
#include "game/world/pool_wrapper.hpp"
#include "game/utility/xml.hpp"


//ComponentFactory is responsible for creating components and returning pointers to them.
//It uses pools to allocate/deallocate memory for components and maintains a separate pool
//for each component type.
class ComponentFactory : boost::noncopyable
{
  private:
    unsigned int index = 0; //Each registered component must have unique index. This holds the value of the next index to use.
    std::unordered_map<std::string, unsigned int> namePoolMap; //Can index into pools from a component name to get a specific pool.
    std::vector<PoolInterface*> pools; //Pointers to the actual pools. //TODO: Update to use std::unique_ptr.
    std::unordered_map<std::string, boost::function<std::shared_ptr<Component::BaseComponent>(Xml) > > makerFunctions; //Map between component names and the correct maker function (instantiation of maker_template<T>).

    //Allocates memory for a new component from the pool, creates it, and returns a shared pointer to it (with correct deleter).
    //Pointers to instantiations are stored in makerFunctions and called by new_component.
    //template <typename ComponentType>
    //std::shared_ptr<ComponentType> maker_template(); //Template function which is used to product functions which can make different kinds of Component.

  public:
    ComponentFactory() {  } //Default constructor.
    //Creates a pool for a given component and stores the component name/index pair.
    template <typename ComponentType>
    void register_component();

    template <typename ComponentType, typename ... OptionalConstructorArgs>
    std::shared_ptr<ComponentType> custom_construct_component(OptionalConstructorArgs ... optionalConstructorArgs);

    //template <typename ComponentType, typename ... OptionalConstructorArgs>
    //std::shared_ptr<ComponentType> maker_template2(OptionalConstructorArgs ... optionalConstructorArgs); //Template function which is used to product functions which can make different kinds of Component.

    //Calls the correct (stored) instantiation of maker_template<ComponentType> to create a new component from the pool. Returns a shared_ptr to the component.
    std::shared_ptr<Component::BaseComponent> new_component(std::string _componentName, Xml _xml);

    ~ComponentFactory(); //Currently needed to just clean up the pools.
};

//Creates a pool for a given component and stores the component name/index pair.
    //TODO: Update to use std::unique_ptr in pools vector.
template <typename ComponentType>
void ComponentFactory::register_component()
{
    namePoolMap[ComponentType::get_name()] = index; //Associate the component name with an index.
    pools.push_back(new PoolWrapper<ComponentType>()); //Create a new pool for the component type.
    index++;

    //Register maker function (allows reflection).
    //Uncomment below line for errors. Check the makerFunction map expects one Xml object argument. Ensure
    makerFunctions[ComponentType::get_name()] = boost::bind(&ComponentFactory::custom_construct_component<ComponentType, Xml>, this, _1);

    ///Testing...
    //boost::function<std::shared_ptr<Component::BaseComponent>(Xml)> func;
    //func = boost::bind(&ComponentFactory::custom_construct_component<ComponentType, Xml>, this, _1);
}

//Allocates memory for a new component from the pool, creates it, and returns a shared pointer to it (with correct deleter).
//Pointers to instantiations are stored in makerFunctions and called by new_component.
template <typename ComponentType, typename ... OptionalConstructorArgs>
std::shared_ptr<ComponentType> ComponentFactory::custom_construct_component(OptionalConstructorArgs ... optionalConstructorArgs)
{
    unsigned int poolIndex = namePoolMap.at(ComponentType::get_name()); //Find the correct index to access the pool with.
    PoolWrapper<ComponentType>* poolWrapper = static_cast<PoolWrapper<ComponentType>*>(pools[poolIndex]); //Pointer to the correct PoolWrapper for ComponentType.

    //Now create the component and return a shared pointer to it.
    std::shared_ptr<ComponentType> component(poolWrapper->get_pool().construct(optionalConstructorArgs ...), poolWrapper->get_deleter()); //Shared pointer to a new object from the pool, with correct pool deleter.
    return component;
}



//Allocates memory for a new component from the pool, creates it, and returns a shared pointer to it (with correct deleter).
//Pointers to instantiations are stored in makerFunctions and called by new_component.
/*template <typename ComponentType>
std::shared_ptr<ComponentType> ComponentFactory::maker_template()
{
    unsigned int poolIndex = namePoolMap.at(ComponentType::get_name()); //Find the correct index to access the pool with.
    PoolWrapper<ComponentType>* poolWrapper = static_cast<PoolWrapper<ComponentType>*>(pools[poolIndex]); //Pointer to the correct PoolWrapper for ComponentType.

    //Now create the component and return a pointer to it.
    std::shared_ptr<ComponentType> component(poolWrapper->get_pool().construct(), poolWrapper->get_deleter()); //Shared pointer to a new object from the pool, with correct pool deleter.
    return component;
}*/

/*

//ComponentFactory is responsible for creating components and returning pointers to them.
//It uses pools to allocate/deallocate memory for components and maintains a separate pool
//for each component type.
class ComponentFactory : boost::noncopyable
{
  private:
    unsigned int index = 0; //Each registered component must have unique index. This holds the value of the next index to use.
    std::unordered_map<std::string, unsigned int> namePoolMap; //Can index into pools from a component name to get a specific pool.
    std::vector<PoolInterface*> pools; //Pointers to the actual pools. //TODO: Update to use std::unique_ptr.
    std::unordered_map<std::string, boost::function<std::shared_ptr<Component::BaseComponent>()>> makerFunctions; //Map between component names and the correct maker function (instantiation of maker_template<T>).

    //Allocates memory for a new component from the pool, creates it, and returns a shared pointer to it (with correct deleter).
    //Pointers to instantiations are stored in makerFunctions and called by new_component.
    template <typename ComponentType>
    std::shared_ptr<ComponentType> maker_template(); //Template function which is used to product functions which can make different kinds of Component.

    template <typename ComponentType>
    std::shared_ptr<ComponentType> maker_template2(); //Template function which is used to product functions which can make different kinds of Component.

  public:
    ComponentFactory() {  } //Default constructor.
    //Creates a pool for a given component and stores the component name/index pair.
    template <typename ComponentType>
    void register_component();

    //Calls the correct (stored) instantiation of maker_template<ComponentType> to create a new component from the pool. Returns a shared_ptr to the component.
    std::shared_ptr<Component::BaseComponent> new_component(std::string _componentName);

    ~ComponentFactory(); //Currently needed to just clean up the pools.
};

//Creates a pool for a given component and stores the component name/index pair.
    //TODO: Update to use std::unique_ptr in pools vector.
template <typename ComponentType>
void ComponentFactory::register_component()
{
    namePoolMap[ComponentType::get_name()] = index; //Associate the component name with an index.
    pools.push_back(new PoolWrapper<ComponentType>()); //Create a new pool for the component type.
    index++;

    //Register maker function
    makerFunctions[ComponentType::get_name()] = boost::bind(&ComponentFactory::maker_template<ComponentType>, this);
}

//Allocates memory for a new component from the pool, creates it, and returns a shared pointer to it (with correct deleter).
//Pointers to instantiations are stored in makerFunctions and called by new_component.
template <typename ComponentType>
std::shared_ptr<ComponentType> ComponentFactory::maker_template2()
{
    unsigned int poolIndex = namePoolMap.at(ComponentType::get_name()); //Find the correct index to access the pool with.
    PoolWrapper<ComponentType>* poolWrapper = static_cast<PoolWrapper<ComponentType>*>(pools[poolIndex]); //Pointer to the correct PoolWrapper for ComponentType.

    //lsdfkj Rewrite below to use optional constructor args. Should still bind to function pointer so that it is quick!
    //Now create the component and return a pointer to it.
    std::shared_ptr<ComponentType> component(poolWrapper->get_pool().construct(), poolWrapper->get_deleter()); //Shared pointer to a new object from the pool, with correct pool deleter.
    return component;
}



//Allocates memory for a new component from the pool, creates it, and returns a shared pointer to it (with correct deleter).
//Pointers to instantiations are stored in makerFunctions and called by new_component.
template <typename ComponentType>
std::shared_ptr<ComponentType> ComponentFactory::maker_template()
{
    unsigned int poolIndex = namePoolMap.at(ComponentType::get_name()); //Find the correct index to access the pool with.
    PoolWrapper<ComponentType>* poolWrapper = static_cast<PoolWrapper<ComponentType>*>(pools[poolIndex]); //Pointer to the correct PoolWrapper for ComponentType.

    //Now create the component and return a pointer to it.
    std::shared_ptr<ComponentType> component(poolWrapper->get_pool().construct(), poolWrapper->get_deleter()); //Shared pointer to a new object from the pool, with correct pool deleter.
    return component;
}

*/
