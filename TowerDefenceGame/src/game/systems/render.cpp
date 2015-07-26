#include "render.hpp"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "game/components/position.hpp"
#include "game/components/renderable.hpp"
#include "game/world/entity_manager.hpp"
#include "game/world/resource_manager.hpp"
#include "game/resources/wrapped_resource.hpp"

#include "game/components/entity_spawner.hpp" //Temp
#include "game/events/trigger_entity_creation_event.hpp" //Temp
#include "game/world/event_bus.hpp"
#include <boost/lexical_cast.hpp>

namespace System
{
    Render::Render(EntityManager* _em, EventBus* _eb, ResourceManager* _resourceManager) : BaseSystem(_em, _eb), resourceManager(_resourceManager)
    {
        systemMask.set(_em->get_component_id<Component::Renderable>());
        systemMask.set(_em->get_component_id<Component::Position>());
    }

    void Render::handle_event(const Event::RenderEvent &_event)
    {
        //For each entity.
        for (Entity iEntity=0; iEntity < em->component_list_size(); iEntity++)
        {
            //If it contains all the required components.
            if (em->match_mask(iEntity, systemMask))
            {
                //Get position and renderable components
                const auto position = em->get_component<Component::Position>(iEntity).lock();
                const auto renderable = em->get_component<Component::Renderable>(iEntity).lock();

                //Draw bitmap.
                if (renderable->is_visible())
                    al_draw_rotated_bitmap(resourceManager->get_resource<WrappedBitmap>(renderable->get_bitmap()), renderable->get_width()/2.0, renderable->get_height()/2.0, position->get_x(), position->get_y(), position->get_orientation(), 0);
            }
        }

        /*
        ///TEMP TESTING CODE. SHOULD BE DELETED! (spawning entities)...
        counter++; //Temp
        if (counter % 100 == 0)
        {
            Entity endEntity = em->component_list_size();
            for (Entity iEntity=0; iEntity < endEntity; iEntity++)
            {
                //Temporary code below to test entity creations...
                if (em->contains_component<Component::EntitySpawner>(iEntity))
                {
                    auto spawner = em->get_component<Component::EntitySpawner>(iEntity).lock();
                    auto pos = em->get_component<Component::Position>(iEntity).lock();
                    float x = pos->get_x(); float y = pos->get_y(); float orientation = pos->get_orientation();
                    std::cout << "y = " << y << std::endl;
                    Event::TriggerEntityCreationEvent tecEvent(resourceManager->get_resource<WrappedXML>(spawner->entitySpecification),
                                                               {{"Entity.Position.<xmlattr>.x", boost::lexical_cast<std::string>(x+10)},
                                                                {"Entity.Position.<xmlattr>.y", boost::lexical_cast<std::string>(y+10)},
                                                                {"Entity.Position.<xmlattr>.orientation", boost::lexical_cast<std::string>(orientation+1.1415)}});
                    eb->send_event<Event::TriggerEntityCreationEvent>(tecEvent);
                }
            }
        }*/
    }
}
