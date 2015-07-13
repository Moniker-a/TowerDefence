#pragma once

#include "component.hpp"
#include "game/utility/xml.hpp"

namespace Component
{
    //Simple component which defines the parameters of attraction.
    struct Attractor : public BaseComponent
    {
        float strength; //Scales strength of attraction.
        static std::string get_name() { return "Attractor"; } //Required.

        //Constructors.
        Attractor(const Xml& _xmlDefinition) :
            strength(_xmlDefinition.get<float>("Entity.Attractor.<xmlattr>.strength"))
            {  }
        Attractor() : strength(1.0) {  }
        Attractor(float _strength) : strength(_strength) {  }
    };
}

