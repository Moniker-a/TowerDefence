#pragma once

#include <iostream>

namespace Component
{
    //Interface class; allows polymorphism.
    class BaseComponent
    {
      private:
        //
      public:
        static std::string get_name() { return "Component"; }
        ~BaseComponent() { std::cout << "Destroyed BaseComponent!" << std::endl; }
    };
}


