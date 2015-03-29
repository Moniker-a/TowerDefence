#pragma once

//World is the main class in which all of the game will reside. It will run the main loop of the game.
//At the moment this is very incomplete and has mostly been used for driving debugging.


class ComponentTypeRegister;

namespace World
{
    const ComponentTypeRegister& get_registry();
    void initalise();
};
