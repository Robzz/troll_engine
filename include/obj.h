#ifndef OBJ_H
#define OBJ_H

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <map>
#include <set>
#include "mesh.h"
#include "obj_lexer.h"
#include "obj_parser.h"

class Obj {
    friend class ObjReader;

    public:
    /* Default constructor */
    Obj();
    /* Destructor */
    virtual ~Obj();

    /* Copy constructor */
    Obj(Obj const& other);
    /* Move constructor */
    Obj(Obj&& other);
    /* Assignment operator */
    Obj& operator=(Obj const& other);
    /* Move-assignment operator */
    Obj& operator=(Obj&& other);

    void add_group(std::string const& name);
    IndexedMesh<unsigned int>* get_group(std::string const& name);
    std::map<std::string, IndexedMesh<unsigned int>> const& groups() const;

    private:
    std::string m_objName;
    std::map<std::string, IndexedMesh<unsigned int>> m_groups;
};

class ObjReader {
    friend class yy::ObjParser;

    public:
    /* Default constructor */
    ObjReader();
    /* Destructor */
    virtual ~ObjReader();

    /* Move constructor */
    ObjReader(ObjReader&& other);
    /* Move-assignment operator */
    ObjReader& operator=(ObjReader&& other);

    /* Specify the obj file to read */
    ObjReader& file(std::string const& file);
    /* Read the specified file */
    Obj read();

    private:
    struct Group {
        std::vector<glm::vec4> vertices;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> texCoords;
        std::vector<Face>      faces;

        Group() : vertices(), normals(), texCoords(), faces() { }
    };
    std::string                  m_filename;
    ObjLexer*                    m_lexer;
    yy::ObjParser*               m_parser;
    std::map<std::string, Group> m_groups;
    Group*                       m_currentGroup;

    /* Bison helper functions */
    void add_vertex(glm::vec3 const& v);
    void add_vertex(glm::vec4 const& v);
    void add_normal(glm::vec3 const& v);
    void add_uv(glm::vec2 const& v);
    void add_face(Face const& f);

    IndexUvNormal duplicate_vertex(IndexUvNormal const& iun);

    /* This class is not copyable*/
    ObjReader(ObjReader const& other);
    ObjReader& operator=(ObjReader const& other);
};

#endif
