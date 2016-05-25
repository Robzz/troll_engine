#ifndef _OBJECT_H
#define _OBJECT_H

#include "component.h"
#include "program.h"
#include "mesh.h"

namespace Engine {

/**
  * \class Object
  * \brief The Object class represents an object in a scene.
  */
class Object {
    public:
        /* Default constructor */
        Object(Mesh const& m, Program const& p);
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
        Program const& m_program;
        VAO const& m_vao;
};

} // namespace Engine

#endif
