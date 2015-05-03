#pragma once
#include <map>
#include <memory>
#include <boost/noncopyable.hpp>
#include "game/systems/system.hpp"


//Forward declarations
class ComponentManager;

//Container for Systems. Responsible for updating Systems in the correct order.
class SystemManager : boost::noncopyable
{
private:
    EntityManager* em; //Pointer to the EntityManager which systems belonging to this SystemManager will process.
    EventBus* eb; //Pointer to the EventBus which systems belonging to this SystemManager will use.

    std::multimap<int, std::unique_ptr<System::BaseSystem>> systemRegister; //Multimap containing a list of systems in priority order
                                                                            //TODO: Consider using a vector and sorting on priority after each add?
    std::map<std::string, System::BaseSystem*> systemTags; //Used to in get_system() get raw pointers to named Systems.

public:
    SystemManager(EntityManager* _em, EventBus* _eb) : em(_em), eb(_eb) {  };
    void update(); //Updates all Systems in order of priority.

    template <class TSystem>
    void register_system(int _priority, const std::string _tag=""); //Registers a System to the manager and assigns it an update priority.

    template <class TSystem>
    TSystem* get_system(const std::string _name); //Returns a const non-owning raw pointer corresponding to a particular named system
};

//Registers a System. This associates it with a priority which determines the order with which it will be updated.
//Systems can optionally be given a name/tag, allowing const pointer access via SystemManager::get_system(_tag).
template <class TSystem>
void SystemManager::register_system(int _priority, const std::string _tag)
{
    //No pointers allowed.
    if (std::is_pointer<TSystem>::value)
        throw std::runtime_error("Error: SystemManager::register_system<T>() - TSystem cannot be a pointer");

    //Must be derived from BaseSystem.
    if (!std::is_base_of<System::BaseSystem,TSystem>::value)
        throw std::runtime_error("Error: SystemManager::register_system<T>() - TSystem must be base of BaseSystem");

    //Register the System by creating a new System and.
    auto addedPair = systemRegister.insert(std::pair<int, std::unique_ptr<System::BaseSystem>>(_priority,  std::unique_ptr<TSystem>(new TSystem(em, eb)))); //Note to replace with std::make_unique if we start using C++14.

    //If _tag isn't empty, add a name entry for the pointer.
    if (_tag != "")
    {
        System::BaseSystem* rawPtr = (*addedPair).second.get(); //Non-owning pointer, so it's fine that it's raw.
        systemTags[_tag] = rawPtr;
    }
}

//Returns a const non-owning raw pointer corresponding to a particular named system.
template <class TSystem>
TSystem* SystemManager::get_system(const std::string _name)
{
    auto pairToGet = systemTags.find(_name);
    if (pairToGet != systemTags.end()) //If we found the named system
        return static_cast<TSystem*>(pairToGet->second); //Return the pointer...
    else
        return nullptr; //Otherwise return a null pointer.
}

