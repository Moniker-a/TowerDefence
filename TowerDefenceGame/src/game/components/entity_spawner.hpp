#pragma once

#include "component.hpp"
#include "game/resources/wrapped_xml.hpp"
#include "game/world/resource_manager.hpp"

namespace Component
{
    // Simple component which holds position data
    struct EntitySpawner : public BaseComponent
    {
        std::string entitySpecification;
        //WrappedXML xmlSpecification;
        //EntitySpawner(const Xml& _xmlDefinition, ResourceManager* _resourceManager) :
        //    entitySpecification(_xmlDefinition.get<std::string>("Entity.EntitySpawner.<xmlattr>.entitySpecification")),
        //    xmlSpecification(_resourceManager->get_resource<WrappedXML>(entitySpecification))
        //    {  }
        EntitySpawner(const Xml& _xmlDefinition) :
            entitySpecification(_xmlDefinition.get<std::string>("Entity.EntitySpawner.<xmlattr>.entitySpecification"))
            {  }
        static std::string get_name() { return "EntitySpawner"; }
    };
}
