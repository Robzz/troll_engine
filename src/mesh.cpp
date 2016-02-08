#include "mesh.h"
#include "debug.h"

Mesh::Mesh() :
    m_vertices(),
    m_normals(),
    m_texCoords()
{ }

Mesh::~Mesh() { }

Mesh::Mesh(Mesh const& other) :
    m_vertices(other.m_vertices),
    m_normals(other.m_normals),
    m_texCoords(other.m_texCoords)
{ }

Mesh::Mesh(Mesh&& other) :
    m_vertices(other.m_vertices),
    m_normals(other.m_normals),
    m_texCoords(other.m_texCoords)
{ }

Mesh& Mesh::operator=(Mesh const& other) {
    m_vertices  = other.m_vertices;
    m_normals   = other.m_normals;
    m_texCoords = other.m_texCoords;
    return *this;
}

Mesh& Mesh::operator=(Mesh&& other) {
    m_vertices  = std::move(other.m_vertices);
    m_normals   = std::move(other.m_normals);
    m_texCoords = std::move(other.m_texCoords);
    return *this;
}

size_t Mesh::n_vertices() const {
    return m_vertices.size();
}

size_t Mesh::n_normals() const {
    return m_normals.size();
}

size_t Mesh::n_texCoords() const {
    return m_texCoords.size();
}

std::vector<glm::vec4> const& Mesh::vertices() const {
    return m_vertices;
}

std::vector<glm::vec3> const& Mesh::normals() const {
    return m_normals;
}

std::vector<glm::vec2> const& Mesh::texCoords() const {
    return m_texCoords;
}

void Mesh::add_vertex(glm::vec4 const& v) {
    m_vertices.push_back(v);
}

void Mesh::add_vertex(glm::vec3 const& v) {
    m_vertices.push_back(glm::vec4(v, 1.f));
}

void Mesh::add_normal(glm::vec3 const& v) {
    m_normals.push_back(v);
}

void Mesh::add_texCoord(glm::vec2 const& v) {
    m_texCoords.push_back(v);
}
