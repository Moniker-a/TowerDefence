#pragma once

//Just contains typedefs for Entity.
//An Entity represents an index which can be used to access all of a particular game object's components from the Component lists in ComponentManager.
//This is guarenteed to stay constant for the lifetime of an entity, and can thus be used as an ID.

typedef unsigned int Entity;
