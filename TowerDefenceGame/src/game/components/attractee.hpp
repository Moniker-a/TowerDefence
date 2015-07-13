#pragma once

#include "component.hpp"
#include "game/utility/xml.hpp"

namespace Component
{
    //Simple component which defines the parameters of attraction.
    struct Attractee : public BaseComponent
    {
        float strength; //Scales strength of attraction.
        static std::string get_name() { return "Attractee"; } //Required.

        //Constructors.
        Attractee(const Xml& _xmlDefinition) :
            strength(_xmlDefinition.get<float>("Entity.Attractee.<xmlattr>.strength"))
            {  }
        Attractee() : strength(1.0) {  }
        Attractee(float _strength) : strength(_strength) {  }
    };
}


