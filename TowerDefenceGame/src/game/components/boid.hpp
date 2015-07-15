#pragma once

namespace Component
{
    struct Boid : public BaseComponent
    {
        float attractRadius;
        float attractStrength;
        float repelRadius;
        float repelStrength;
        float reorientRadius;
        float reorientStrength;
        float maxVelocity;

        static std::string get_name() { return "Boid"; }

        //Constructors.
        Boid(const Xml& _xmlDefinition) :
            attractRadius(_xmlDefinition.get<float>("Entity.Boid.<xmlattr>.attractRadius")),
            attractStrength(_xmlDefinition.get<float>("Entity.Boid.<xmlattr>.attractStrength")),
            repelRadius(_xmlDefinition.get<float>("Entity.Boid.<xmlattr>.repelRadius")),
            repelStrength(_xmlDefinition.get<float>("Entity.Boid.<xmlattr>.repelStrength")),
            reorientRadius(_xmlDefinition.get<float>("Entity.Boid.<xmlattr>.reorientRadius")),
            reorientStrength(_xmlDefinition.get<float>("Entity.Boid.<xmlattr>.reorientStrength")),
            maxVelocity(_xmlDefinition.get<float>("Entity.Boid.<xmlattr>.maxVelocity"))
            {  }
        Boid() : attractRadius(50.0), attractStrength(1.0), repelRadius(20.0), repelStrength(3.0), reorientRadius(35.0), reorientStrength(1.0), maxVelocity(1.0) {  }
    };
}
