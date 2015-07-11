#pragma once

#include <string>
#include "component.hpp"
#include "game/utility/xml.hpp"

namespace Component
{
    // Simple component which holds position data
    class Renderable : public BaseComponent
    {
    private:
        std::string bitmap;
        float width, height;
        bool visible;
    public:
        Renderable(Xml _xml) :
            bitmap(_xml.get<std::string>("Entity.Renderable.<xmlattr>.bitmap")),
            width(_xml.get<float>("Entity.Renderable.<xmlattr>.width")),
            height(_xml.get<float>("Entity.Renderable.<xmlattr>.height")),
            visible(_xml.get("Entity.Renderable.<xmlattr>.visible", true))
            {  }
        Renderable() : bitmap("default bitmap"), width(50), height(80), visible(true) {  }
        static std::string get_name() { return "Renderable"; }
        bool is_visible() const { return visible; }
        std::string get_bitmap() const { return bitmap; }
        float get_width() const { return width; }
        float get_height() const { return height; }
    };
}



