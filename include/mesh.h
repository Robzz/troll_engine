#ifndef MESH_H
#define MESH_H

#include <vector>
#include <glm/glm.hpp>

/* This class contains mesh geometry. */
class Mesh {
    public:
    /* Default constructor */
    Mesh();
    /* Destructor */
    virtual ~Mesh();

    /* Copy constructor */
    Mesh(Mesh const& other);
    /* Move constructor */
    Mesh(Mesh&& other);
    /* Assignment operator */
    void operator=(Mesh const& other);
    /* Move-assignment operator */
    void operator=(Mesh&& other);

    protected:
        std::vector<glm::vec4> m_vertices;
};

/* This class contains mesh geometry in indexed form. */
template <class T>
class IndexedMesh : public Mesh {
    public:
    /* Default constructor */
    IndexedMesh();
    /* Destructor */
    virtual ~IndexedMesh();

    /* Copy constructor */
    IndexedMesh(IndexedMesh const& other);
    /* Move constructor */
    IndexedMesh(IndexedMesh&& other);
    /* Assignment operator */
    void operator=(IndexedMesh const& other);
    /* Move-assignment operator */
    void operator=(IndexedMesh&& other);

    private:
        std::vector<T> m_indices;
};

#endif
