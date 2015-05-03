#pragma once

#include <string>
#include "component.hpp"

namespace Component
{
    //Component which determines whether or not an entity's position is wrapped around camera field of view.
    class BoundingBox : public BaseComponent
    {
    private:
        //

    public:
        int x1, y1, x2, y2; //Made these public. Seemed stupid to use accessor methods for such a trivial thing...

        BoundingBox(int _x1=0, int _y1=0, int _x2=10, int _y2=10) : x1(_x1), y1(_y1), x2(_x2), y2(_y2) {  }
        static std::string get_name() { return "BoundingBox"; }

        /*int get_x1() const { return x1; }
        int get_y1() const { return y1; }
        int get_x2() const { return x2; }
        int get_y2() const { return y2; }
        void set_x1(int _x1) { x1 = _x1; }
        void set_y1(int _y1) { y1 = _y1; }
        void set_x2(int _x2) { x2 = _x2; }
        void set_y2(int _y2) { y2 = _y2; }*/
    };
}


