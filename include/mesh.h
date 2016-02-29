#ifndef MESH_H
#define MESH_H

#include <vector>
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>
#include "attribute.h"

namespace Engine {

typedef std::unordered_map<std::string, AttributeBase*> AttributeMap;

/* This class contains mesh geometry and attributes. */
class Mesh {
    public:
    /* Destructor */
    virtual ~Mesh();

    /* Copy constructor */
    Mesh(Mesh const& other);
    /* Move constructor */
    Mesh(Mesh&& other);
    /* Assignment operator */
    Mesh& operator=(Mesh const& other);
    /* Move-assignment operator */
    Mesh& operator=(Mesh&& other);

    template <class T>
    Attribute<T>* get_attribute(std::string const& name) {
        Attribute<T>* ptr = dynamic_cast<Attribute<T>*>(m_attributes[name]);
        if(ptr)
            return ptr;
        else {
            throw std::runtime_error("Invalid attribute type");
        }
    }

    template <class T>
    Attribute<T> const* get_attribute(std::string const& name) const {
        Attribute<T>* ptr = dynamic_cast<Attribute<T>*>(m_attributes.at(name));
        if(ptr)
            return ptr;
        else {
            throw std::runtime_error("Invalid attribute type");
        }
    }

    protected:
        friend class MeshBuilder;
        explicit Mesh(AttributeMap const& attrmap);

        AttributeMap m_attributes;
};

class MeshBuilder {
    public:
    /* Default constructor */
    MeshBuilder();
    /* Destructor */
    virtual ~MeshBuilder();

    template<class T>
    MeshBuilder& with_attribute(std::string const& name) {
        if(m_attributes.count(name)) {
            throw std::runtime_error("Attribute already present");
        }
        else {
            m_attributes.insert(std::pair<std::string, AttributeBase*>(name, new Attribute<T>()));
            return *this;
        }
    }

    Mesh* build_mesh() const;

    private:
    /* This class is not copyable. */
    MeshBuilder(MeshBuilder const& other);
    MeshBuilder& operator=(MeshBuilder const& other);
    
    AttributeMap m_attributes;
};

} // namespace Engine

#endif
