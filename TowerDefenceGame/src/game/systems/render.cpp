#include "render.hpp"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "game/components/position.hpp"
#include "game/components/renderable.hpp"
#include "game/world/entity_manager.hpp"
#include "game/world/resource_manager.hpp"
#include "game/resources/wrapped_resource.hpp"

namespace System
{
    Render::Render(EntityManager* _em, EventBus* _eb, ResourceManager* _resourceManager) : BaseSystem(_em, _eb), resourceManager(_resourceManager)
    {
        systemMask.set(_em->get_component_id<Component::Renderable>());
        systemMask.set(_em->get_component_id<Component::Position>());
    }

    void Render::handle_event(const RenderEvent &_event)
    {
        //For each entity.
        for (Entity iEntity=0; iEntity < em->component_list_size(); iEntity++)
        {
            //If it contains all the required components.
            if (em->match_mask(iEntity, systemMask))
            {
                //Get position and renderable components
                auto position = em->get_component<Component::Position>(iEntity).lock();
                auto renderable = em->get_component<Component::Renderable>(iEntity).lock();

                //Draw bitmap.
                if (renderable->is_visible())
                {
                    //std::cout << "Render System rendering " << iEntity << "at (" << position->get_x() << ", " << position->get_y() << ")\n";
                    al_draw_bitmap(resourceManager->get_resource<WrappedBitmap>(renderable->get_bitmap()), position->get_x(), position->get_y(), 0);
                }
            }
        }
    }
}
