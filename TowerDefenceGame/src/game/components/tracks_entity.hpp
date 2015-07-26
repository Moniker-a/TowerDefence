#pragma once

#include "component.hpp"

namespace Component
{
    //Component specifying that the entity tracks (reorients toward) another entity.
    struct TracksEntity : public BaseComponent
    {
        Entity toTrack;
        float strength; //Scales strength of tracking (i.e. how quickly the self entity is reoriented).
        static std::string get_name() { return "TracksEntity"; } //Required.

        //Constructors.
        TracksEntity(const Xml& _xmlDefinition) :
            toTrack(_xmlDefinition.get<Entity>("Entity.Attractor.<xmlattr>.toTrack")),
            strength(_xmlDefinition.get<float>("Entity.Attractor.<xmlattr>.strength"))
            {  }
    };
}
