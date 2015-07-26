#pragma once

#include "component.hpp"

namespace Component
{
    //Simple component which defines the parameters of attraction.
    struct DebugInfo : public BaseComponent
    {
        std::string name; //Tag we can use to hook into other components.
        static std::string get_name() { return "DebugInfo"; } //Required.

        //Constructors.
        DebugInfo(const Xml& _xmlDefinition) :
            name(_xmlDefinition.get<std::string>("Entity.DebugInfo.<xmlattr>.name"))
            {  }
    };
}



