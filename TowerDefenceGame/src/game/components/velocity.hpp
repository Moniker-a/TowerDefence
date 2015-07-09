#pragma once

#include <string>

#include "component.hpp"

namespace Component
{
    // Simple component which holds velocity data
    class Velocity : public BaseComponent
    {
      private:
          float dx;
          float dy;

      public:
        Velocity() : dx(10), dy(1.5) {  }
        Velocity(float _dx, float _dy) : dx(_dx), dy(_dy) {  }
        static std::string get_name() { return "Velocity"; }
        float get_dx() const { return dx; }
        float get_dy() const { return dy; }
        void set_dx(float _dx) { dx = _dx; }
        void set_dy(float _dy) { dy = _dy; }
    };
}


