#pragma once

#include <unordered_map>
#include <string>
#include <boost/bimap.hpp>
#include <boost/bimap/unordered_set_of.hpp>
#include <boost/noncopyable.hpp>

// Component ID type
typedef unsigned int ComponentID;

//This class is used to register a component type.
//Components must be registered before any entities are created so that ComponentManager knows how many component lists to make.
//Keeps track of the string names of registered Components which and converts between Component type ID and string name.
class ComponentTypeRegister : boost::noncopyable
{
  private:
    ComponentID nextID=0; //Used to assign keys (Component IDs).
    typedef boost::bimap<boost::bimaps::unordered_set_of<ComponentID>, boost::bimaps::unordered_set_of<std::string>> ComponentRegistry;
    ComponentRegistry componentRegistry; //Bimap between ComponentIDs and their string names.

  public:
    ComponentTypeRegister() {  }; //Default constructor.
    unsigned int get_size() const { return nextID; }; //Returns number of registered components
    void add(const std::string _label); //Registers Component of given name.
    std::string get_label(const ComponentID _ID) const; //Gets component name from ComponentID.
    ComponentID get_ID(const std::string _label) const; //Gets ComponentID from name.
    bool exists(const ComponentID _ID) const; //Returns true if a component with given ComponentID has been registered.
    bool exists(const std::string _label) const; //Returns true if a component with given string name has been registered.

    template <class T>
    ComponentID get_ID() const { return get_ID(T::get_name()); } //Returns the ComponentID from the component name (Components know their own name).
};
