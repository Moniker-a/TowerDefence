#pragma once

#include <string>
#include "component.hpp"

//Temp headers
#include <allegro5/allegro5.h>

namespace Component
{
    // Simple component which holds position data
    class Renderable : public BaseComponent
    {
      private:
        bool visible;
        ALLEGRO_BITMAP* image;
        unsigned int length, width;
      public:
        Renderable() : visible(true), length(50), width(80) { image = al_load_bitmap("resources/mushroomeer.png"); }
        ~Renderable() {al_destroy_bitmap(image);} //Temp
        static std::string get_name() { return "Renderable"; }
        bool is_visible() const { return visible; }
        ALLEGRO_BITMAP* get_image() const { return image; }
    };
}



