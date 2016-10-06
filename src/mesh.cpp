#include "mesh.h"
#include "debug.h"
#include "scenegraph.h"

using namespace gl;
using namespace std;

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
const char* Mesh::name() const { return m_name.c_str(); }

DrawableNode* Mesh::instantiate(glm::mat4 const& position, Program* p, Texture const* tex,
                                GLenum primitiveMode) const {
    VAO* vao = new VAO();
    GLuint attrIndex;
    attrIndex = static_cast<unsigned int>(p->getAttributeLocation("v_position"));
    const AttributeArray::Layout* l = &m_attribs.positions.layout;
    vao->enableVertexAttribArray(attrIndex);
    vao->vertexAttribPointer(*m_attribs.positions.vbo, attrIndex, l->nComponents(),
                             l->stride(), reinterpret_cast<void*>(l->offset()),
                             traits::gl_value<AttributeArray::Type>::value(l->type()), l->normalize());
    if(m_attribs.normals) {
        l = &(*m_attribs.normals).layout;
        attrIndex = static_cast<unsigned int>(p->getAttributeLocation("v_normal"));
        vao->enableVertexAttribArray(attrIndex);
        vao->vertexAttribPointer(*(*m_attribs.normals).vbo, attrIndex, l->nComponents(),
                                 l->stride(), reinterpret_cast<void*>(l->offset()),
                                 traits::gl_value<AttributeArray::Type>::value(l->type()), l->normalize());
    }
    if(m_attribs.colors) {
        l = &(*m_attribs.colors).layout;
        attrIndex = static_cast<unsigned int>(p->getAttributeLocation("v_color"));
        vao->enableVertexAttribArray(attrIndex);
        vao->vertexAttribPointer(*(*m_attribs.colors).vbo, attrIndex, l->nComponents(),
                                 l->stride(), reinterpret_cast<void*>(l->offset()),
                                 traits::gl_value<AttributeArray::Type>::value(l->type()), l->normalize());
    }
    if(m_attribs.uvs) {
        l = &(*m_attribs.uvs).layout;
        attrIndex = static_cast<unsigned int>(p->getAttributeLocation("v_texCoord"));
        vao->enableVertexAttribArray(attrIndex);
        vao->vertexAttribPointer(*(*m_attribs.uvs).vbo, attrIndex, l->nComponents(),
                                 l->stride(), reinterpret_cast<void*>(l->offset()),
                                 traits::gl_value<AttributeArray::Type>::value(l->type()), l->normalize());
    }
    if(isIndexed()) {
        return new IndexedObject(position, p, (*m_attribs.indices).vbo, vao, m_nIndices, tex,
                                 traits::gl_value<AttributeArray::Type>::value((*m_attribs.indices).layout.type()),
                                 primitiveMode);
    }
    else {
        // TODO : assuming triangles for now
        return new Object(position, p, vao, m_nVertices / 3, tex, primitiveMode);
    }
}

unsigned int Mesh::numVertices() const { return m_nVertices; }
unsigned int Mesh::numFaces() const { return m_nIndices; }

std::unique_ptr<Mesh> Mesh::quad() {
    MeshBuilder mb("Quad");
    mb.vertices({glm::vec3(-0.5f, 0.5f, 0.f), glm::vec3(0.5f, 0.5f, 0.f),
                 glm::vec3(0.5f, -0.5f, 0.f), glm::vec3(-0.5f, -0.5f, 0.f)});
    mb.normals({glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 0.f, 1.f),
                glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 0.f, 1.f)});
    mb.uvs({glm::vec2(0.f, 1.f), glm::vec2(1.f, 1.f), glm::vec2(1.f, 0.f),
            glm::vec2(0.f, 0.f)});
    mb.faces(std::vector<unsigned char>({0, 2, 1, 0, 3, 2}));
    return mb.build_mesh();
}

std::unique_ptr<Mesh> Mesh::quadStrip(vector<glm::vec2> const& uvs) {
    if(uvs.size() % 4) {
        throw runtime_error("Wrong number of UV coordinates");
    }
    int nQuads = uvs.size() / 4;
    vector<glm::vec3> vertices, normals;
    vector<unsigned int> indices;
    vertices.reserve(4*nQuads);
    normals.reserve(4*nQuads);
    indices.reserve(6*nQuads);
    for(int i = 0 ; i != nQuads ; ++i) {
        float fi = i;
        unsigned int indicesBufLen = i * 4;
        vertices.insert(vertices.end(),
                        {glm::vec3(fi      , 1.f, 0.f), glm::vec3(fi + 1.f, 1.f, 0.f),
                         glm::vec3(fi + 1.f, 0.f, 0.f), glm::vec3(fi      , 0.f, 0.f)}
        );
        normals.insert(normals.end(),
                       {glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 0.f, 1.f),
                        glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 0.f, 1.f)}
        );
        indices.insert(indices.end(),
                       {indicesBufLen + 0, indicesBufLen + 2, indicesBufLen + 1,
                        indicesBufLen + 0, indicesBufLen + 3, indicesBufLen + 2}
        );
    }
    MeshBuilder mb("QuadStrip");
    mb.vertices(vertices);
    mb.normals(normals);
    mb.uvs(uvs);
    mb.faces(std::move(indices));
    return mb.build_mesh();
}

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
    auto vbo = std::make_unique<VBO>();
    vbo->upload_data(verts);
    m_nVertices = verts.size();
    m_meshAttribs.positions = AttributeArray(*vbo,
                                             AttributeArray::Kind::Positions,
                                             AttributeArray::Layout(3, AttributeArray::Type::Float));
    m_resources.push_back(std::move(vbo));
    return *this;
}

MeshBuilder& MeshBuilder::normals(std::vector<glm::vec3> const& norms) {
    auto vbo = std::make_unique<VBO>();
    vbo->upload_data(norms);
    m_nNormals = norms.size();
    m_meshAttribs.normals =
        std::make_unique<AttributeArray>(*vbo,
                                         AttributeArray::Kind::Normals,
                                         AttributeArray::Layout(3, AttributeArray::Type::Float));
    m_resources.push_back(std::move(vbo));
    return *this;
}

MeshBuilder& MeshBuilder::colors(std::vector<glm::vec4> const& cols) {
    auto vbo = std::make_unique<VBO>();
    vbo->upload_data(cols);
    m_nColors = cols.size();
    m_meshAttribs.colors =
        std::make_unique<AttributeArray>(*vbo,
                                         AttributeArray::Kind::Colors,
                                         AttributeArray::Layout(4, AttributeArray::Type::Float));
    m_resources.push_back(std::move(vbo));
    return *this;
}

MeshBuilder& MeshBuilder::uvs(std::vector<glm::vec2> const& uvs) {
    auto vbo = std::make_unique<VBO>();
    vbo->upload_data(uvs);
    m_nUVs = uvs.size();
    m_meshAttribs.uvs =
        std::make_unique<AttributeArray>(*vbo,
                                         AttributeArray::Kind::UVs,
                                         AttributeArray::Layout(2, AttributeArray::Type::Float));
    m_resources.push_back(std::move(vbo));
    return *this;
}

MeshBuilder& MeshBuilder::uvs(std::vector<glm::vec3> const& uvs) {
    auto vbo = std::make_unique<VBO>();
    vbo->upload_data(uvs);
    m_nUVs = uvs.size();
    m_meshAttribs.uvs =
        std::make_unique<AttributeArray>(*vbo,
                                         AttributeArray::Kind::UVs,
                                         AttributeArray::Layout(3, AttributeArray::Type::Float));
    m_resources.push_back(std::move(vbo));
    return *this;
}

MeshBuilder& MeshBuilder::faces(std::vector<unsigned char>&& indices) {
    auto vbo = std::make_unique<VBO>();
    vbo->upload_data(indices);
    m_nIndices = indices.size();
    m_meshAttribs.indices =
        std::make_unique<AttributeArray>(*vbo,
                                         AttributeArray::Kind::Indices,
                                         AttributeArray::Layout(0, AttributeArray::Type::Uchar));
    m_resources.push_back(std::move(vbo));
    return *this;
}

MeshBuilder& MeshBuilder::faces(std::vector<unsigned short>&& indices) {
    auto vbo = std::make_unique<VBO>();
    vbo->upload_data(indices);
    m_nIndices = indices.size();
    m_meshAttribs.indices =
        std::make_unique<AttributeArray>(*vbo,
                                         AttributeArray::Kind::Indices,
                                         AttributeArray::Layout(0, AttributeArray::Type::Ushort));
    m_resources.push_back(std::move(vbo));
    return *this;
}

MeshBuilder& MeshBuilder::faces(std::vector<unsigned int>&& indices) {
    auto vbo = std::make_unique<VBO>();
    vbo->upload_data(indices);
    m_nIndices = indices.size();
    m_meshAttribs.indices =
        std::make_unique<AttributeArray>(*vbo,
                                         AttributeArray::Kind::Indices,
                                         AttributeArray::Layout(0, AttributeArray::Type::Uint));
    m_resources.push_back(std::move(vbo));
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
