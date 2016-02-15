#include "mesh.h"
#include "debug.h"

Mesh::Mesh(AttributeMap const& attrmap) :
    m_attributes(attrmap)
{
    for(auto& it : attrmap) {
        m_attributes.insert(std::pair<std::string, AttributeBase*>(it.first, it.second));
    }
}

Mesh::~Mesh() {
    for(auto& it : m_attributes) {
        delete it.second;
    }
}

Mesh::Mesh(Mesh const& other) :
    m_attributes(other.m_attributes)
{ }

Mesh::Mesh(Mesh&& other) :
    m_attributes(other.m_attributes)
{ }

Mesh& Mesh::operator=(Mesh const& other) {
    m_attributes = other.m_attributes;
    return *this;
}

Mesh& Mesh::operator=(Mesh&& other) {
    m_attributes = std::move(other.m_attributes);
    return *this;
}

MeshBuilder::MeshBuilder() :
    m_attributes()
{ }

MeshBuilder::~MeshBuilder() {
    for(auto& it : m_attributes) {
        delete it.second;
    }
}

Mesh* MeshBuilder::build_mesh() const {
    AttributeMap copy = m_attributes;
    for(auto& it : copy) {
        it.second = it.second->copy();
    }
    return new Mesh(copy);
}
