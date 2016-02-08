#ifndef MESH_H
#define MESH_H

#include <vector>
#include <glm/glm.hpp>

/* This class contains mesh geometry and attributes. */
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
    Mesh& operator=(Mesh const& other);
    /* Move-assignment operator */
    Mesh& operator=(Mesh&& other);

    size_t n_vertices() const;
    size_t n_normals() const;
    size_t n_texCoords() const;

    std::vector<glm::vec4> const& vertices() const;
    std::vector<glm::vec3> const& normals() const;
    std::vector<glm::vec2> const& texCoords() const;

    void add_vertex(glm::vec4 const& v);
    void add_vertex(glm::vec3 const& v);
    void add_normal(glm::vec3 const& v);
    void add_texCoord(glm::vec2 const& v);

    protected:
        std::vector<glm::vec4> m_vertices;
        std::vector<glm::vec3> m_normals;
        std::vector<glm::vec2> m_texCoords;
};

/* This class contains mesh geometry in indexed form. */
template <class T>
class IndexedMesh : public Mesh {
    public:
    /* Default constructor */
    IndexedMesh() :
        Mesh(),
        m_indices()
    { }
    /* Destructor */
    virtual ~IndexedMesh() { }

    /* Copy constructor */
    IndexedMesh(IndexedMesh const& other) :
        Mesh(other),
        m_indices(other.m_indices)
    { }
    /* Move constructor */
    IndexedMesh(IndexedMesh&& other) :
        Mesh(other),
        m_indices(other.m_indices)
    { }
    /* Assignment operator */
    IndexedMesh& operator=(IndexedMesh const& other) {
        *this = dynamic_cast<Mesh const&>(other);
        m_indices = other.m_indices;
        return *this;
    }
    /* Move-assignment operator */
    IndexedMesh& operator=(IndexedMesh&& other) {
        *this = std::move(dynamic_cast<Mesh const&>(other));
        m_indices = std::move(other.m_indices);
        return *this;
    }

    size_t n_indices() const { return m_indices.size(); }
    std::vector<unsigned int> const& indices() const { return m_indices; }
    void add_index(unsigned int i) { m_indices.push_back(i); }

    private:
        std::vector<T> m_indices;
};

#endif
