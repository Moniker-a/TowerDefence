#pragma once

#include <string>
#include "component.hpp"
#include "game/utility/xml.hpp"

#include <iostream> //temp

namespace Component
{
    // Simple component which holds position data
    class Position : public BaseComponent
    {
    private:
        float x;
        float y;

    public:
        Position(const Xml& _xmlDefinition) :
            x(_xmlDefinition.get<float>("Entity.Position.<xmlattr>.x")),
            y(_xmlDefinition.get<float>("Entity.Position.<xmlattr>.y"))
            { std::cout << "constructed Position with xml constructor!" << std::endl; }
        Position() : x(100), y(100) { std::cout << "constructed Position with default constructor..." << std::endl; }
        Position(float _x, float _y) : x(_x), y(_y) { std::cout << "constructed Position with (float, float) constructor: (" << x << ", " << y << ")." << std::endl; }
        static std::string get_name() { return "Position"; }
        float get_x() const { return x; }
        float get_y() const { return y; }
        void set_x(float _x) { x = _x; }
        void set_y(float _y) { y = _y; }
    };
}
