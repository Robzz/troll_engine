#ifndef MESH_COMPONENT_H
#define MESH_COMPONENT_H

#include "component.h"

namespace Engine {

/**
  * \class MeshComponent
  * \brief The MeshComponent class is the component of an object which contains
  * its geometry and attributes.
  */
class MeshComponent : ComponentBase {
    public:
        /* Default constructor */
        MeshComponent();
        /* Destructor */
        virtual ~MeshComponent();

        /* Copy constructor */
        MeshComponent(MeshComponent const& other);
        /* Move constructor */
        MeshComponent(MeshComponent&& other);
        /* Assignment operator */
        MeshComponent& operator=(MeshComponent const& other);
        /* Move-assignment operator */
        MeshComponent& operator=(MeshComponent&& other);


    private:
        unsigned int m_nUvChannels; 
};

} // namespace Engine

#endif
