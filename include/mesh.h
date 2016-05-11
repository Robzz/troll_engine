#ifndef MESH_H
#define MESH_H

#include <vector>
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>
#include <memory>
#include "attribute.h"

namespace Engine {

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

        unsigned int numVertices() const;
        unsigned int numFaces() const;

    protected:
        friend class MeshBuilder;
        Mesh(std::string const& name);

        std::string m_name;
        std::unique_ptr<AttributeArrayInstance<glm::vec3>> m_vertices;
        std::unique_ptr<AttributeArrayInstance<glm::vec3>> m_normals;
        std::unique_ptr<AttributeArrayInstance<glm::vec4>> m_colors;
        std::unique_ptr<AttributeArray> m_uvs;
        std::unique_ptr<AttributeArray> m_faces;

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

        MeshBuilder& vertices(std::vector<glm::vec3>&& verts);
        MeshBuilder& normals(std::vector<glm::vec3>&& norms);
        MeshBuilder& colors(std::vector<glm::vec4>&& cols);
        MeshBuilder& faces(std::vector<glm::vec4>&& cols);

        std::unique_ptr<Mesh> build_mesh();

        /* No copy or move */
        MeshBuilder(MeshBuilder const& other) = delete;
        MeshBuilder& operator=(MeshBuilder const& other) = delete;
        MeshBuilder(MeshBuilder&& other) = delete;
        MeshBuilder& operator=(MeshBuilder&& other) = delete;

    private:
        std::unique_ptr<Mesh> m_mesh;

        void validateMesh() const;

        template <typename T>
        MeshBuilder& _initAttribArray(std::vector<T> vec, std::unique_ptr<AttributeArrayInstance<T>>& ptr,
                                      std::string const& errMsg);
};

#include "mesh.inl"

} // namespace Engine

#endif
