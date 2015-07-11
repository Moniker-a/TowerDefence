#pragma once
//#include <game/world/resource_manager.hpp>

class ComponentTypeRegister;
class ResourceManager;

namespace World
{
    const ComponentTypeRegister& get_registry();
    void initalise(ResourceManager* _resourceManager);
    void update();
    void render();
};
