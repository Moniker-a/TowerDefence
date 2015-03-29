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
    //Multimap contains a list of systems in priority order
        //TODO: Consider using a vector and sorting on priority after each add?
    std::multimap<int, std::unique_ptr<System::BaseSystem>> systemRegister;

  public:
    SystemManager() {  };
    void update(EntityManager &_em); //Updates all Systems in order of priority.

    template <class TSystem>
    void register_system(int _priority); //Registers a System to the manager and assigns it an update priority.
};

// implementation of register system
template <class TSystem>
void SystemManager::register_system(int _priority)
{
    //No pointers allowed.
    if (std::is_pointer<TSystem>::value)
        throw std::runtime_error("Error: SystemManager::register_system<T>() - TSystem cannot be a pointer");

    //Must be derived from BaseSystem.
    if (!std::is_base_of<System::BaseSystem,TSystem>::value)
        throw std::runtime_error("Error: SystemManager::register_system<T>() - TSystem must be base of BaseSystem");

    //Register the System by creating a new System and.
    systemRegister.insert(std::pair<int, std::unique_ptr<System::BaseSystem>>(_priority,  std::unique_ptr<TSystem>(new TSystem()))); //Note to replace with std::make_unique if we start using C++14.
}

