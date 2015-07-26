#pragma once

#include "component.hpp"
#include "game/utility/xml.hpp"

namespace Component
{
    //Simple component which defines the parameters of attraction.
    struct SelfPropelled : public BaseComponent
    {
        float acceleration; //Scales strength of attraction.
        static std::string get_name() { return "SelfPropelled"; } //Required.

        //Constructors.
        SelfPropelled(const Xml& _xmlDefinition) :
            acceleration(_xmlDefinition.get<float>("Entity.SelfPropelled.<xmlattr>.acceleration"))
            {  }
    };
}


