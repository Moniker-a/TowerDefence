#pragma once

#include <string>
#include "component.hpp"

namespace Component
{
    // Simple component which holds position data
    class Position : public BaseComponent
    {
      private:
        float x;
        float y;

      public:
        Position() : x(100), y(100) {  }
        Position(float _x, float _y) : x(_x), y(_y) {  }
        static std::string get_name() { return "Position"; }
        float get_x() const { return x; }
        float get_y() const { return y; }
        void set_x(float _x) { x = _x; }
        void set_y(float _y) { y = _y; }
    };
}


