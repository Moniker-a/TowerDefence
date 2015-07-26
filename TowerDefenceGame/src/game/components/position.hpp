#pragma once

#include "component.hpp"
#include "game/utility/xml.hpp"

namespace Component
{
    // Simple component which holds position data
    struct Position : public BaseComponent
    {
    public:
        float x;
        float y;
        float orientation; //In radians.

        Position(const Xml& _xmlDefinition) :
            x(_xmlDefinition.get<float>("Entity.Position.<xmlattr>.x")),
            y(_xmlDefinition.get<float>("Entity.Position.<xmlattr>.y")),
            orientation(_xmlDefinition.get<float>("Entity.Position.<xmlattr>.orientation"))
            {  }
        //Position() : x(100), y(100), orientation(0) {  }
        //Position(float _x, float _y) : x(_x), y(_y) {  }
        static std::string get_name() { return "Position"; }
        float get_x() const { return x; }
        float get_y() const { return y; }
        float get_orientation() const { return orientation; }
        void set_x(float _x) { x = _x; }
        void set_y(float _y) { y = _y; }
        void set_orientation(float _orientation) { orientation = _orientation; }
    };
}
