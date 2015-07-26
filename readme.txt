


Areas for improvement:
1) When updating, Systems currently must iterate through all entities regardless as to whether they're inactive ('deleted') or active. Perhaps we could write a custom iterator which skips over these. These would basically be an iterator which dereferences to an 'Entity' (unsigned int) which can be used like an index in calls to EntityManager::destroy_entity, EntityManager::get_component, etc.. This would also give us an easy way to use range-based loops on EntityManager instead of iterating manually.

2) Flexible attractor types. The loop calculating ddx and ddy could (should?) be handled by a function pointer, allowing different methods for calculating attraction between two objects, and could be stored as a Resource, e.g. AttractorKernal.

3) Component xml constructors should substitute default values or throw exceptions if something is missing from xml.

4) Decide whether xml constructors should be passed just the component info they require (may require extra copying)? Or should they have the whole entity specification (allowing checking for dependancies (e.g. if a SelfPropelling component is being constructed in an entity which doesn't have a Position component throw exception?).
