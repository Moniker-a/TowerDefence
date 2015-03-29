#include "component_factory.hpp"

//Allocates memory for a new component from the pool, creates it, and returns a shared pointer to it (with correct deleter).
std::shared_ptr<Component::BaseComponent> ComponentFactory::new_component(std::string _componentName)
{
    //Checks that the component has a maker function
    if (makerFunctions.find(_componentName) == makerFunctions.end())
        throw std::runtime_error("Error: ComponentFactory::new_component<T>(std::string _componentName) - No maker function found in component_factory. Has the ComponentType been registered?");

    //Run maker function from and return the pointer.
    return makerFunctions.at(_componentName)();
}

//TODO: update to use unique_ptr.
ComponentFactory::~ComponentFactory()
{
    //Clean up pools.
    for (auto itr = pools.begin(); itr != pools.end(); ++itr)
        delete *itr;
    pools.clear();
}
