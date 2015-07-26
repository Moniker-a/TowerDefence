#pragma once

#include "game/components/component.hpp"

namespace Component
{
    class LocallyAware : public BaseComponent
    {
    private:
        std::vector<float> localDistances;
        std::vector<float> localDirections;
    public:
        float radius;
        bool cacheDistance;
        bool cacheDirection;
        std::vector<Entity> localEntities;

        static std::string get_name() { return "LocallyAware"; }

        //Constructors.
        LocallyAware(const Xml& _xmlDefinition) :
            radius(_xmlDefinition.get<float>("Entity.LocallyAware.<xmlattr>.radius")),
            cacheDistance(_xmlDefinition.get<bool>("Entity.LocallyAware.<xmlattr>.cacheDistance")),
            cacheDirection(_xmlDefinition.get<bool>("Entity.LocallyAware.<xmlattr>.cacheDirection"))
            {  }
        LocallyAware() : radius(0), cacheDistance(false), cacheDirection(false) {  }

        inline std::vector<float>& get_distances()
        {
            if (cacheDistance)
                return localDistances;
            else
                throw std::runtime_error("Attempted to manipulate cached distance vector in a Component::LocallyAware which is not caching distances.\n");
        }

        inline std::vector<float>& get_directions()
        {
            if (cacheDistance)
                return localDirections;
            else
                throw std::runtime_error("Attempted to manipulate cached direction vector in a Component::LocallyAware which is not caching directions.\n");
        }
    };
}
