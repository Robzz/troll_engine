#ifndef MESH_H
#define MESH_H

#include <vector>
#include <string>
#include <memory>

#include <glm/glm.hpp>

#include "attribute.h"
#include "program.h"

namespace Engine {

class DrawableNode;
class Texture;

/* This class contains mesh geometry and attributes. */
class Mesh {
    public:
        /* Destructor */
        virtual ~Mesh();

        /* Move constructor */
        Mesh(Mesh&& other);
        /* Move-assignment operator */
        Mesh& operator=(Mesh&& other);

        bool hasNormals() const;
        bool hasColors() const;
        bool hasUVs() const;
        bool isIndexed() const;
        const char* name() const;

        DrawableNode* instantiate(glm::mat4 const& position, Program* p, Texture const* tex = nullptr,
                                  GLenum primitiveMode = GL_TRIANGLES) const;

        unsigned int numVertices() const;
        unsigned int numFaces() const;

    protected:
        friend class MeshBuilder;
        Mesh(std::string const& name, AttributeMap attribs, std::vector<std::unique_ptr<VBO>>&& resources,
             unsigned int nVerts, unsigned int nIndices);

        std::string m_name;
        AttributeMap m_attribs;
        std::vector<std::unique_ptr<VBO>> m_resources;
        const unsigned int m_nVertices, m_nIndices;

        /* No copy */
        Mesh(Mesh const& other) = delete;
        Mesh& operator=(Mesh const& other) = delete;
};

class MeshBuilder {
    public:
        /* Default constructor */
        MeshBuilder(std::string const& meshName = "default");
        /* Destructor */
        virtual ~MeshBuilder();

        MeshBuilder& vertices(std::vector<glm::vec3> const& verts);
        MeshBuilder& normals(std::vector<glm::vec3> const& norms);
        MeshBuilder& colors(std::vector<glm::vec4> const& cols);
        MeshBuilder& uvs(std::vector<glm::vec2> const& uvs);
        MeshBuilder& uvs(std::vector<glm::vec3> const& uvs);
        MeshBuilder& faces(std::vector<unsigned char>&& indices);
        MeshBuilder& faces(std::vector<unsigned short>&& indices);
        MeshBuilder& faces(std::vector<unsigned int>&& indices);

        std::unique_ptr<Mesh> build_mesh();

        /* No copy or move */
        MeshBuilder(MeshBuilder const& other) = delete;
        MeshBuilder& operator=(MeshBuilder const& other) = delete;
        MeshBuilder(MeshBuilder&& other) = delete;
        MeshBuilder& operator=(MeshBuilder&& other) = delete;

    private:
        std::string m_meshName;
        AttributeMap m_meshAttribs;
        std::vector<std::unique_ptr<VBO>> m_resources;
        unsigned int m_nVertices, m_nNormals, m_nColors, m_nUVs, m_nIndices;

        void validateMesh() const;
};

} // namespace Engine

#endif
