#pragma once
#include "game/events/event.hpp"
#include "game/resources/wrapped_xml.hpp"

namespace Event
{
    struct TriggerEntityCreationEvent : public BaseEvent
    {
        WrappedXML entitySpecification;

        TriggerEntityCreationEvent(WrappedXML _entitySpecification, std::vector<std::pair<std::string, std::string>> _mods) : entitySpecification(_entitySpecification)
        {
            for (std::pair<std::string, std::string> mod : _mods)
                entitySpecification.set_attribute(mod.first, mod.second);
        }
    };
}
