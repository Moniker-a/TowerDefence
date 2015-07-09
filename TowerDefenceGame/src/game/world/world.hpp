#pragma once

class ComponentTypeRegister;

namespace World
{
    const ComponentTypeRegister& get_registry();
    void initalise();
    void update();
    void render();
};
