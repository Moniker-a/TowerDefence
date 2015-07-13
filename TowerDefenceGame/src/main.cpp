#include <iostream> //Temp
#include "game/world/world.hpp" //Temp
#include "game/game.hpp"

#include "game/world/event_bus.hpp" //temp?
#include "game/systems/collision_resolver.hpp" //temp?

#include "game/components/position.hpp"
#include "game/utility/xml.hpp"
#include <boost/property_tree/xml_parser.hpp>

#include "game/world/pool_wrapper.hpp"
#include "game/world/component_factory.hpp"


int main()
{
    //Xml xml;
    //boost::property_tree::xml_parser::read_xml("resources/xml/test_entity.xml", xml);
    //Component::Position pos(xml);

    //PoolWrapper<Component::Position> pw;
    //Component::Position* pos = pw.get_pool().construct();
    //Component::Position* pos2 = pw.get_pool().construct(10, 40);
    //Component::Position* pos3 = pw.get_pool().construct(xml);
    //Component::Position* pos4 = pw.get_pool().malloc();

    //ComponentFactory cf;
    //cf.register_component<Component::Position>();
    //std::shared_ptr<Component::Position> position1(cf.custom_construct_component<Component::Position>());
    //std::shared_ptr<Component::Position> position2(cf.custom_construct_component<Component::Position, float, float>(99, 101));



    Game game;
    game.initialise();
    game.start();
    game.exit();

    return 0;
}
