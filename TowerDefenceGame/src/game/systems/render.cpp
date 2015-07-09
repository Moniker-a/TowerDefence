#include "render.hpp"
#include <allegro5/allegro.h>
#include "game/components/position.hpp"
#include "game/components/renderable.hpp"
#include "game/world/entity_manager.hpp"

namespace System
{
    Render::Render(EntityManager* _em, EventBus* _eb) : BaseSystem(_em, _eb)
    {
        systemMask.set(World::get_registry().get_ID<Component::Renderable>());
        systemMask.set(World::get_registry().get_ID<Component::Position>());
    }

    void Render::handle_event(const RenderEvent &_event)
    {
        //For each entity.
        for (Entity iEntity=0; iEntity < em->get_component_list_size(); iEntity++)
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
                    std::cout << "Render System rendering " << iEntity << "at (" << position->get_x() << ", " << position->get_y() << ")\n";
                    al_draw_bitmap(renderable->get_image(), position->get_x(), position->get_y(), 0);
                }
            }
        }
    }
}
