#pragma once

#include "game/events/event.hpp"

namespace Event
{
    //Simple event which allows GameStates to communicate that it is time to render to rendering Systems without the need for hard-coded links between GameState and System.
    class RenderEvent : public BaseEvent
    {
    private:
        //
    public:
        RenderEvent() {  };
    };
}
