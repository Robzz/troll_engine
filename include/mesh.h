/**
  * \file mesh.h
  * \brief Definitionn of mesh related classes
  * \author R.Chavignat
  */
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

/**
  * \class Mesh
  * \brief Contains the geometry and attributes of a mesh.
  */
class Mesh {
    public:
        /**
          * \brief Destructor
          */
        virtual ~Mesh();

        /**
          * \brief Move constructor
          */
        Mesh(Mesh&& other);
        /**
          * \brief Move-assignment operator
          */
        Mesh& operator=(Mesh&& other);

        /**
          * \brief Return true if the mesh has normals.
          */
        bool hasNormals() const;
        /**
          * \brief Return true if the mesh has vertex colors.
          */
        bool hasColors() const;
        /**
          * \brief Return true if the mesh has texture coordinates.
          */
        bool hasUVs() const;
        /**
          * \brief Return true if the mesh has indices.
          */
        bool isIndexed() const;
        /**
          * \brief Return the name of the mesh.
          */
        const char* name() const;

        /**
          * \brief Return an Object that is an instance of the mesh.
          * TODO : refactor this
          */
        DrawableNode* instantiate(glm::mat4 const& position, Program* p, Texture const* tex = nullptr,
                                  gl::GLenum primitiveMode = gl::GL_TRIANGLES) const;

        /**
          * \brief Return the number of vertices in the mesh.
          */
        unsigned int numVertices() const;
        /**
          * \brief Return the number of faces in the mesh.
          */
        unsigned int numFaces() const;

    protected:
        friend class MeshBuilder;
        /**
          * \brief Constructor.
          * \param name %Mesh name
          * \param attribs %Mesh attribute map
          * \param resources Array of VBOs containing the resources that are exclusively part of the
          * Mesh, so they can be free'd on destruction.
          * \param nVerts Number of vertices in the Mesh.
          * \param nIndices Number of indices in the Mesh, if indexed.
          */
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

/**
  * \class MeshBuilder
  * \brief Mesh factory class.
  */
class MeshBuilder {
    public:
        /**
          * \brief Constructor
          */
        explicit MeshBuilder(std::string const& meshName = "default");
        /**
          * \brief Destructor
          */
        virtual ~MeshBuilder();

        /**
          * \brief Specify the Mesh vertices.
          */
        MeshBuilder& vertices(std::vector<glm::vec3> const& verts);
        /**
          * \brief Specify the Mesh normals.
          */
        MeshBuilder& normals(std::vector<glm::vec3> const& norms);
        /**
          * \brief Specify the Mesh vertex colors.
          */
        MeshBuilder& colors(std::vector<glm::vec4> const& cols);
        /**
          * \brief Specify the Mesh 2D texture coordinates.
          */
        MeshBuilder& uvs(std::vector<glm::vec2> const& uvs);
        /**
          * \brief Specify the Mesh 3D texture coordinates.
          */
        MeshBuilder& uvs(std::vector<glm::vec3> const& uvs);
        /**
          * \brief Specify the Mesh indices.
          */
        MeshBuilder& faces(std::vector<unsigned char>&& indices);
        /**
          * \brief Specify the Mesh indices.
          */
        MeshBuilder& faces(std::vector<unsigned short>&& indices);
        /**
          * \brief Specify the Mesh indices.
          */
        MeshBuilder& faces(std::vector<unsigned int>&& indices);

        /**
          * \brief Build the mesh.
          */
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
