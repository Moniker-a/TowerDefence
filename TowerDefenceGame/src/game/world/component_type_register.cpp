#include "component_type_register.hpp"
#include "game/components/component.hpp"

//Returns true if a component with given ComponentID has been registered.
bool ComponentTypeRegister::exists(const ComponentID _ID) const
{
    if (componentRegistry.left.find(_ID) != componentRegistry.left.end()) // is the ID in the map?
        return true;
    else
        return false;
}

//Returns true if a component with given string name has been registered.
bool ComponentTypeRegister::exists(const std::string _label) const
{
    if (componentRegistry.right.find(_label) != componentRegistry.right.end()) // is the string in the map?
        return true;
    else
        return false;
}

//Registers Component with a given name.
void ComponentTypeRegister::add(const std::string _label)
{
    // prevents registering base component or base system
    if (_label == Component::BaseComponent::get_name() || _label == "System") //TODO: Change to "System" to System::BaseSystem::get_name(). It's best to avoid using literals in case they change.
        throw std::runtime_error("ComponentTypeRegister::add: Can not register base component/system\n");     //This way if we want to change it it's just one line. Although... don't we want separate registrars for systems and components?

    // prevents registering the same component twice
    if (exists(_label))
        throw std::runtime_error("ComponentTypeRegister::add: Can not register the same component/system twice (" + _label + ").\n");

    componentRegistry.insert({nextID, _label});
    nextID++;
}

//Gets component name from ComponentID.
std::string ComponentTypeRegister::get_label(const ComponentID _ID) const
{
    if (!exists(_ID))
        throw std::runtime_error("Given component label does not exist\n");
    return componentRegistry.left.at(_ID);
}

//Gets ComponentID from name.
ComponentID ComponentTypeRegister::get_ID(const std::string _label) const
{
    if (!exists(_label))
        throw std::runtime_error("Given component ID, \""+_label+"\" does not exist. Has the component type been registered?\n");
    return componentRegistry.right.at(_label);
}

