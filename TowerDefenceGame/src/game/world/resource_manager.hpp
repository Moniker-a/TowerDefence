#pragma once
#include <unordered_map>
#include <memory>
#include <iostream> //temp
#include <boost/noncopyable.hpp>

//Manages game resources (stores, removes and provides access to game resources of any type).
class ResourceManager : boost::noncopyable
{
private:
    //Nested resource class provides a generic interface for storing game resources of different types by implementing type erasure.
    //Not exposed outside of ResourceManager.
    class Resource
    {
    private:
        //Allows mixed storage in STL containers (not technically polymorphism except in destructor).
        class ResourceConcept
        {
        public:
            virtual ~ResourceConcept() {  }
        };

        //Templated model of ResourceConcept. Allows type erasure and storing of any type.
        template <class ResourceType>
        class ResourceModel : public ResourceConcept
        {
        private:
            ResourceType modelledResource;
        public:
            ResourceModel(const ResourceType &_resource) : modelledResource(_resource) {  }
            const ResourceType& get_resource() const { return modelledResource; } //Self-explanatory.
            virtual ~ResourceModel() {  }
        };

        //Unique pointer to the resource itself. Points to an object of type ResourceConcept allowing any specific instantiation of the templated ResourceModel to be stored.
        std::unique_ptr<ResourceConcept> resource;

    public:
        //Constructor which initialises a resource with whichever ResourceModel is required.
        template <class ResourceType>
        Resource(const ResourceType& _resource) : resource(new ResourceModel<ResourceType>(_resource)) {  }

        //Reverses type erasure to provide correctly typed const pointer to the raw resource.
        template <class ResourceType>
        const ResourceType& unwrap() const
        {
            return dynamic_cast<const ResourceModel<ResourceType>*>(resource.get())->get_resource(); //Faster type safety by manual checking against typeMap?
        }
    };

    std::unordered_map<std::string, Resource> resourceMap;

public:
    //Using variadic templates this is basically an AnythingFactory. It will construct any type of object given the required parameters, then adds them to a single unordered_map.
    template <class ResourceType, class ... ConstructorArgTypes>
    void add_resource(std::string _tag, ConstructorArgTypes... constructorArgTypes)
    {
        std::cout << _tag << std::endl;
        resourceMap.emplace(std::make_pair(_tag, Resource(ResourceType(constructorArgTypes...))));
    }

    //Retrieved a named resource using a key / tag. Returns the resource as a const raw pointer (no ownership).
    template <class ResourceType>
    const ResourceType& get_resource(const std::string _key) const //Should return const raw const pointers.
    {
        auto itr = resourceMap.find(_key);
        if (itr != resourceMap.end())
            return itr->second.unwrap<ResourceType>(); //Return the value, unwrapped to the correct resource type.
        else
            throw std::out_of_range("ResourceManager::get_resource: Resource not found with key/tag " + _key + ".\n");
    }

    //Remove a specified resource.
    void erase_resource(const std::string _key)
    {
        //resourceMap::iterator itr = resourceMap.find(_key);
        auto itr = resourceMap.find(_key);
        if (itr != resourceMap.end())
            resourceMap.erase(itr);
        else
            throw std::runtime_error("ResourceManager::erase_resource: Resource " + _key + " not found and could not be erased.\n");
    }

    //Clears all resources.
    void clear() { resourceMap.clear(); }

    ~ResourceManager()
    {
        std::cout << "destructing ResourceManager" << std::endl;
        clear();
    }
};
