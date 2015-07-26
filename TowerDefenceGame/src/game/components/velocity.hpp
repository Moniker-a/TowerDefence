#pragma once

#include <string>

#include "component.hpp"

namespace Component
{
    //Simple component which holds velocity data
    class Velocity : public BaseComponent
    {
    public: //Todo: remove accessor methods.
        float dx;
        float dy;
        float maxVelocity; //Negative values indicate no maximum velocity.

        Velocity(const Xml& _xmlDefinition) :
            dx(_xmlDefinition.get<float>("Entity.Velocity.<xmlattr>.dx")),
            dy(_xmlDefinition.get<float>("Entity.Velocity.<xmlattr>.dy")),
            maxVelocity(_xmlDefinition.get<float>("Entity.Velocity.<xmlattr>.maxVelocity"))
            {  }
        Velocity() : dx(10), dy(1.5), maxVelocity(-1) {  }
        Velocity(float _dx, float _dy, float _maxVelocity) : dx(_dx), dy(_dy), maxVelocity(_maxVelocity) {  }
        static std::string get_name() { return "Velocity"; }
        float get_dx() const { return dx; }
        float get_dy() const { return dy; }
        void set_dx(float _dx) { dx = _dx; }
        void set_dy(float _dy) { dy = _dy; }
    };
}


