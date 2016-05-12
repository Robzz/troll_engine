#ifndef _OBJECT_H
#define _OBJECT_H

#include "component.h"

namespace Engine {

/**
  * \class Object
  * \brief The Object class represents an object in a scene.
  */
class Object {
    public:
        /* Default constructor */
        Object();
        /* Destructor */
        virtual ~Object();

        /* Copy constructor */
        Object(Object const& other);
        /* Move constructor */
        Object(Object&& other);
        /* Assignment operator */
        Object& operator=(Object const& other);
        /* Move-assignment operator */
        Object& operator=(Object&& other);

    private:
};

} // namespace Engine

#endif
