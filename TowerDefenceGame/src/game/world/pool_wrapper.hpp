#pragma once

#include "game/world/pool_interface.hpp"
#include <boost/pool/object_pool.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>

//Template class inheriting from PoolInterface (to allow polymorphism). Wraps a pool which is used to creating / deallocate ComponentType objects.
//Also holds a function pointer to the deleter for the given ComponentType, encapsulating everything needed to make shared_ptrs from the pool.
template <typename ComponentType>
class PoolWrapper : public PoolInterface
{
  private:
    boost::object_pool<ComponentType> pool; //Pool for creating objects of type ComponentType.
    boost::function<void(ComponentType*)> deleter; //Function pointer to the deleter function for the pool (so that shared_ptrs release memory back to the pool).

  public:
    PoolWrapper() : deleter(boost::bind(&boost::object_pool<ComponentType>::destroy, &pool, _1))
        {  }
    boost::object_pool<ComponentType> &get_pool() { return pool; } //Returns reference to the pool.
    boost::function<void(ComponentType*)> get_deleter() { return deleter; } //Returns function pointer to the pool's deleter.
};
