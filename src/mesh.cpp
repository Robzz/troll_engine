#include "mesh.h"
#include "debug.h"

namespace Engine {

Mesh::Mesh(std::string const& name, AttributeMap attribs, std::vector<std::unique_ptr<VBO>>&& resources,
           unsigned int nVerts, unsigned int nIndices) :
    m_name(name),
    m_attribs(std::move(attribs)),
    m_resources(std::move(resources)),
    m_nVertices(nVerts),
    m_nIndices(nIndices) { }

Mesh::~Mesh() { }

Mesh::Mesh(Mesh&& other) :
    m_name(std::move(other.m_name)),
    m_attribs(std::move(other.m_attribs)),
    m_resources(std::move(other.m_resources)),
    m_nVertices(other.m_nVertices),
    m_nIndices(other.m_nIndices) { }

Mesh& Mesh::operator=(Mesh&& other) {
    m_name = std::move(other.m_name);
    m_attribs = std::move(other.m_attribs);
    m_resources = std::move(other.m_resources);
    return *this;
}

bool Mesh::hasNormals() const { return m_attribs.normals.get(); }
bool Mesh::hasColors() const { return m_attribs.colors.get(); }
bool Mesh::hasUVs() const { return m_attribs.uvs.get(); }
bool Mesh::isIndexed() const { return m_nIndices; }

unsigned int Mesh::numVertices() const { return m_nVertices; }
unsigned int Mesh::numFaces() const { return m_nIndices; }

MeshBuilder::MeshBuilder(std::string const& name) :
    m_meshName(name),
    m_meshAttribs(),
    m_resources(),
    m_nVertices(0),
    m_nNormals(0),
    m_nColors(0),
    m_nUVs(0),
    m_nIndices(0) { }

MeshBuilder::~MeshBuilder() { }

MeshBuilder& MeshBuilder::vertices(std::vector<glm::vec3> const& verts) {
    VBO vbo;
    vbo.upload_data(verts);
    m_nVertices = verts.size();
    m_meshAttribs.positions = AttributeArray(vbo,
                                         AttributeArray::Kind::Positions,
                                         AttributeArray::Layout(3, AttributeArray::Type::Float));
    return *this;
}

MeshBuilder& MeshBuilder::normals(std::vector<glm::vec3> const& norms) {
    VBO vbo;
    vbo.upload_data(norms);
    m_nNormals = norms.size();
    m_meshAttribs.normals =
        std::make_unique<AttributeArray>(vbo,
                                         AttributeArray::Kind::Normals,
                                         AttributeArray::Layout(3, AttributeArray::Type::Float));
    return *this;
}

MeshBuilder& MeshBuilder::colors(std::vector<glm::vec4> const& cols) {
    VBO vbo;
    vbo.upload_data(cols);
    m_nColors = cols.size();
    m_meshAttribs.colors =
        std::make_unique<AttributeArray>(vbo,
                                         AttributeArray::Kind::Colors,
                                         AttributeArray::Layout(4, AttributeArray::Type::Float));
    return *this;
}

MeshBuilder& MeshBuilder::uvs(std::vector<glm::vec2> const& uvs) {
    VBO vbo;
    vbo.upload_data(uvs);
    m_nUVs = uvs.size();
    m_meshAttribs.uvs =
        std::make_unique<AttributeArray>(vbo,
                                         AttributeArray::Kind::UVs,
                                         AttributeArray::Layout(2, AttributeArray::Type::Float));
    return *this;
}

MeshBuilder& MeshBuilder::uvs(std::vector<glm::vec3> const& uvs) {
    VBO vbo;
    vbo.upload_data(uvs);
    m_nUVs = uvs.size();
    m_meshAttribs.uvs =
        std::make_unique<AttributeArray>(vbo,
                                         AttributeArray::Kind::UVs,
                                         AttributeArray::Layout(3, AttributeArray::Type::Float));
    return *this;
}

MeshBuilder& MeshBuilder::faces(std::vector<unsigned char>&& indices) {
    VBO vbo;
    vbo.upload_data(indices);
    m_nIndices = indices.size();
    m_meshAttribs.indices =
        std::make_unique<AttributeArray>(vbo,
                                         AttributeArray::Kind::Indices,
                                         AttributeArray::Layout(0, AttributeArray::Type::Uchar));
    return *this;
}

MeshBuilder& MeshBuilder::faces(std::vector<unsigned short>&& indices) {
    VBO vbo;
    vbo.upload_data(indices);
    m_nIndices = indices.size();
    m_meshAttribs.indices =
        std::make_unique<AttributeArray>(vbo,
                                         AttributeArray::Kind::Indices,
                                         AttributeArray::Layout(0, AttributeArray::Type::Ushort));
    return *this;
}

MeshBuilder& MeshBuilder::faces(std::vector<unsigned int>&& indices) {
    VBO vbo;
    vbo.upload_data(indices);
    m_nIndices = indices.size();
    m_meshAttribs.indices =
        std::make_unique<AttributeArray>(vbo,
                                         AttributeArray::Kind::Indices,
                                         AttributeArray::Layout(0, AttributeArray::Type::Uint));
    return *this;
}

std::unique_ptr<Mesh> MeshBuilder::build_mesh() {
    validateMesh();
    std::unique_ptr<Mesh> mesh;
    mesh.reset(new Mesh(m_meshName, std::move(m_meshAttribs), std::move(m_resources), m_nVertices, m_nIndices));
    m_nVertices = 0;
    return mesh;
}

void MeshBuilder::validateMesh() const {
    if(!m_nVertices)
        throw std::runtime_error("Mesh has no geometry");
    if(m_nNormals && !(m_nVertices == m_nNormals))
        throw std::runtime_error("Mesh doesn't have the same number of vertices and normals");
    if(m_nColors && !(m_nVertices == m_nColors))
        throw std::runtime_error("Mesh doesn't have the same number of vertices and colors");
    if(m_nUVs && !(m_nVertices == m_nUVs))
        throw std::runtime_error("Mesh doesn't have the same number of vertices and UVs");
    if(m_nIndices && m_nIndices % 3)
        throw std::runtime_error("Mesh number of vertex indices is not a multiple of 3");
}

} // namespace Engine
