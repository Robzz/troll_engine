#include "attribute.h"

namespace Engine {

AttributeArray::Layout::Layout() :
    m_nComponents(0),
    m_type(AttributeArray::Type::Float),
    m_stride(0),
    m_normalize(false),
    m_offset(0) { }

AttributeArray::Layout::Layout(int n, AttributeArray::Type type, size_t s, bool norm, std::intptr_t o) :
    m_nComponents(n),
    m_type(type),
    m_stride(s),
    m_normalize(norm),
    m_offset(o) { }

AttributeArray::Layout::Layout(AttributeArray::Layout const& other) :
    m_nComponents(other.m_nComponents),
    m_type(other.m_type),
    m_stride(other.m_stride),
    m_normalize(other.m_normalize),
    m_offset(other.m_offset) { }

AttributeArray::Layout& AttributeArray::Layout::operator=(AttributeArray::Layout const& other) {
    m_nComponents = other.m_nComponents;
    m_type = other.m_type;
    m_stride = other.m_stride;
    m_normalize = other.m_normalize;
    m_offset = other.m_offset;

    return *this;
}

int AttributeArray::Layout::nComponents() const { return m_nComponents; }
AttributeArray::Type AttributeArray::Layout::type() const { return m_type; }
size_t AttributeArray::Layout::stride() const { return m_stride; }
bool AttributeArray::Layout::normalize() const { return m_normalize; }
std::intptr_t AttributeArray::Layout::offset() const { return m_offset; }

AttributeArray::AttributeArray() :
    vbo(nullptr),
    kind(),
    layout()
{ }

AttributeArray::AttributeArray(VBO const& _vbo, AttributeArray::Kind k, AttributeArray::Layout const& l) :
    vbo(&_vbo),
    kind(k),
    layout(l)
{ }

AttributeArray::AttributeArray(AttributeArray&& other) :
    vbo(other.vbo),
    kind(other.kind),
    layout(other.layout)
{ }

AttributeArray::~AttributeArray() { }

AttributeArray& AttributeArray::operator=(AttributeArray&& other) {
    vbo = other.vbo;
    kind = other.kind;
    layout = other.layout;
    return *this;
}

AttributeMap::AttributeMap() :
    positions(),
    normals(),
    colors(),
    indices(),
    uvs() { }

AttributeMap::AttributeMap(AttributeMap&& other) :
    positions(std::move(other.positions)),
    normals(std::move(other.normals)),
    colors(std::move(other.colors)),
    indices(std::move(other.indices)),
    uvs(std::move(other.uvs)) { }

AttributeMap& AttributeMap::operator=(AttributeMap&& other) {
    positions = std::move(other.positions),
    normals = std::move(other.normals),
    colors = std::move(other.colors),
    indices = std::move(other.indices),
    uvs = std::move(other.uvs);
    return *this;
}

} // namespace Engine
