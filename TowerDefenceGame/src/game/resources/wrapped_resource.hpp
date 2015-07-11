#include <memory>
#include <allegro.h>
#include <allegro_image.h>
#include <allegro_font.h>

//Wrapper for opaque structures/objects and other objects which require non-default destruction.
//Ensures correct deletion with shared ownership.
//Internal class 'InnerWrapper' wraps the actual resource and ensures correct creation using provided creator_func, and destruction using provided deleter_func.
//WrappedResource then uses a std::shared_ptr to decide when to destroy InnerWrapper, allowing use of the WrappedResource freely without need to worry about copying or memory management.
template <class WrappedResourceType, class CreatorFunctionPtrType, CreatorFunctionPtrType creator_func, void (*deleter_func)(WrappedResourceType*), class... Args>
class WrappedResource
{
private:
    //InnerWrapper wraps the actual resource and ensures correct creation using provided creator_func, and destruction using provided deleter_func.
    class InnerWrapper
    {
    private:
        WrappedResourceType* wrappedResource; //No const, see e.g. http://alleg.sourceforge.net/stabledocs/en/const.html#BITMAP%20objects
    public:
        InnerWrapper(Args... args) : wrappedResource(creator_func(args...)) {  } //Involks creator_func to create an opaque object.
        ~InnerWrapper() { deleter_func(wrappedResource); } //
        WrappedResourceType* unwrap() const { return wrappedResource; }
    };

    //WrappedResource uses a shared_ptr<InnerWrapper> to free user of worrying about copying or memory management.
    //Ensure InnerWrapper (and thus the wrappedResource) is deleted at the correct time.
    std::shared_ptr<InnerWrapper> inner;

public:
    WrappedResource(Args... args) : inner(std::make_shared<InnerWrapper>(args...)) {  } //Variadic template parameters passed to the InnerWrapper which uses them in creator_func to correctly create opaque objects.
    operator WrappedResourceType* () const { return inner->unwrap(); } //Conversion from wrapper type to wrapped type. Allows objects of WrappedResource to be used in functions which required the resource which have been wrapped.
    //WrappedResource(const WrappedResource& _other) : inner(_other.inner) {  } //Copy constructor (not strictly needed I think default would work).
    //WrappedResource(const WrappedResource* _other) : inner(_other->inner) {  } //Copy* constructor (not strictly needed I think default would work).
    //WrappedResource& operator= (const WrappedResource &_other) { inner = _other.inner; return *this; } //Assignment operator (not strictly needed I think default would work).
};

//Template parameters are non-trivial, define some easy type handles.
typedef WrappedResource<ALLEGRO_BITMAP, decltype(&al_load_bitmap), al_load_bitmap, al_destroy_bitmap, const char*> WrappedBitmap;
typedef WrappedResource<ALLEGRO_FONT, decltype(&al_load_font), al_load_font, al_destroy_font, const char*, int, int> WrappedFont;
#include "wrapped_xml.hpp"
