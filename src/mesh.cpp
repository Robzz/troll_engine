#include "mesh.h"
#include "debug.h"

namespace Engine {

Mesh::Mesh(std::string const& name) :
    m_name(name),
    m_vertices(),
    m_normals(),
    m_colors(),
    m_uvs(),
    m_faces() { }

Mesh::~Mesh() { }

Mesh::Mesh(Mesh&& other) :
    m_name(std::move(other.m_name)),
    m_vertices(std::move(other.m_vertices)),
    m_normals(std::move(other.m_normals)),
    m_colors(std::move(other.m_colors)),
    m_uvs(std::move(other.m_uvs)),
    m_faces(std::move(other.m_faces)) { }

Mesh& Mesh::operator=(Mesh&& other) {
    m_name = std::move(other.m_name);
    m_vertices = std::move(other.m_vertices);
    m_normals = std::move(other.m_normals);
    m_colors = std::move(other.m_colors);
    m_uvs = std::move(other.m_uvs);
    m_faces = std::move(other.m_faces);
    return *this;
}

bool Mesh::hasNormals() const { return static_cast<bool>(m_normals); }
bool Mesh::hasColors() const { return static_cast<bool>(m_colors); }
bool Mesh::hasUVs() const { return static_cast<bool>(m_uvs); }
bool Mesh::isIndexed() const { return static_cast<bool>(m_faces); }

unsigned int Mesh::numVertices() const { return m_vertices->size(); }
unsigned int Mesh::numFaces() const { return m_faces->size() / 3; }

MeshBuilder::MeshBuilder(std::string const& name) :
    m_mesh(std::unique_ptr<Mesh>(new Mesh(name)))
{ }

MeshBuilder::~MeshBuilder() { }

MeshBuilder& MeshBuilder::vertices(std::vector<glm::vec3>&& verts) {
    return _initAttribArray<glm::vec3>(verts, m_mesh->m_vertices, "Mesh already has geometry");
}

MeshBuilder& MeshBuilder::normals(std::vector<glm::vec3>&& norms) {
    _initAttribArray<glm::vec3>(norms, m_mesh->m_normals, "Mesh already has normals");
    return *this;
}

MeshBuilder& MeshBuilder::colors(std::vector<glm::vec4>&& cols) {
    _initAttribArray<glm::vec4>(cols, m_mesh->m_colors, "Mesh already has colors");
    return *this;
}

std::unique_ptr<Mesh> MeshBuilder::build_mesh() {
    if(!m_mesh)
        throw std::runtime_error("Mesh was already built");
    validateMesh();
    return std::move(m_mesh);
}

void MeshBuilder::validateMesh() const {
    if(!m_mesh->m_vertices)
        throw std::runtime_error("Mesh has no geometry");
    if(m_mesh->hasNormals() && !(m_mesh->m_vertices->size() == m_mesh->m_normals->size()))
        throw std::runtime_error("Mesh doesn't have the same number of vertices and normals");
    if(m_mesh->hasColors() && !(m_mesh->m_vertices->size() == m_mesh->m_colors->size()))
        throw std::runtime_error("Mesh doesn't have the same number of vertices and colors");
    if(m_mesh->hasUVs() && !(m_mesh->m_vertices->size() == m_mesh->m_uvs->size()))
        throw std::runtime_error("Mesh doesn't have the same number of vertices and UVs");
    if(m_mesh->isIndexed() && m_mesh->m_faces->size() % 3)
        throw std::runtime_error("Mesh number of vertex indices is not a multiple of 3");
}

} // namespace Engine
