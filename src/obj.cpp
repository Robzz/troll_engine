#include "obj.h"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <functional>
#include "obj_parser.h"

Obj::Obj() :
    m_objName(),
    m_groups()
{ }

Obj::~Obj() {

}

Obj::Obj(Obj const& other) :
    m_objName(other.m_objName),
    m_groups(other.m_groups)    
{ }

Obj::Obj(Obj&& other) :
    m_objName(std::move(other.m_objName)),
    m_groups(std::move(other.m_groups))
{ }

Obj& Obj::operator=(Obj const& other) {
    m_objName = other.m_objName;
    m_groups = other.m_groups;

    return *this;
}

Obj& Obj::operator=(Obj&& other) {
    m_objName = std::move(other.m_objName);
    m_groups = std::move(other.m_groups);

    return *this;
}

void Obj::add_group(std::string const& name) {
    if(m_groups.count(name)) {
        throw std::runtime_error("Group already exists");
    }
    else {
        MeshBuilder mb;
        mb.with_attribute<unsigned int>("indices")
          .with_attribute<glm::vec4>   ("vertices")
          .with_attribute<glm::vec3>   ("normals")
          .with_attribute<glm::vec2>   ("texCoords");
        m_groups.insert(std::pair<std::string, Mesh*>(name, mb.build_mesh()));
    }
}

Mesh* Obj::get_group(std::string const& name) {
    auto it = m_groups.find(name);
    if(it == m_groups.end()) {
        return nullptr;
    }
    return (*it).second;
}

std::map<std::string, Mesh*> const& Obj::groups() const {
    return m_groups;
}

ObjReader::ObjReader() :
    m_filename(),
    m_lexer(nullptr),
    m_parser(nullptr),
    m_groups(),
    m_currentGroup(nullptr)
{ }

ObjReader::~ObjReader() {
    if(m_lexer)
        delete m_lexer;
    if(m_parser)
        delete m_parser;
}

ObjReader::ObjReader(ObjReader&& other) :
    m_filename(std::move(other.m_filename)),
    m_lexer(other.m_lexer),
    m_parser(other.m_parser),
    m_groups(other.m_groups),
    m_currentGroup(other.m_currentGroup)
{
    other.m_lexer        = nullptr;
    other.m_parser       = nullptr;
    other.m_currentGroup = nullptr;
}

ObjReader& ObjReader::operator=(ObjReader&& other) {
    m_filename = std::move(other.m_filename);
    m_lexer = other.m_lexer;
    m_parser = other.m_parser;
    m_groups = std::move(other.m_groups);
    m_currentGroup = other.m_currentGroup;

    other.m_lexer        = nullptr;
    other.m_parser       = nullptr;
    other.m_currentGroup = nullptr;
    return *this;
}

ObjReader& ObjReader::file(std::string const& filename) {
    m_filename = filename;
    return *this;
}

Obj ObjReader::read() {
    std::ifstream in(m_filename);
    m_lexer = new ObjLexer(&in);
    m_parser = new yy::ObjParser(*m_lexer, *this);
    m_groups.insert(std::pair<std::string, Group>("default", Group()));
    m_currentGroup = &(*(m_groups.find("default"))).second;

    //m_parser->set_debug_level(1);

    if(m_parser->parse() != 0) {
        throw std::runtime_error("Cannot parse obj file");
    }

    Obj obj;
    for(auto &group: m_groups) {
        obj.add_group(group.first);
        Mesh* m(obj.get_group(group.first));
        std::map<IndexUvNormal, unsigned int, std::function<bool(IndexUvNormal const&, IndexUvNormal const&)>> unique_vertices([] (IndexUvNormal const& e1, IndexUvNormal const& e2) {
            if(e1.index == e2.index) {
                if(e1.uv == e2.uv) {
                    return e1.normal < e2.normal;
                }
                else {
                    return e1.uv < e2.uv;
                }
            }
            else {
                return e1.index < e2.index;
            }
        });
        unsigned int index = 0;
        Attribute<unsigned int>* indices = m->get_attribute<unsigned int>("indices");
        Attribute<glm::vec4>*   vertices = m->get_attribute<glm::vec4>("vertices");
        Attribute<glm::vec3>*   normals  = m->get_attribute<glm::vec3>("normals");
        Attribute<glm::vec2>*  texCoords = m->get_attribute<glm::vec2>("texCoords");
        for(auto &face: group.second.faces) {
            auto f = [&] (IndexUvNormal const& iun) {
                if(unique_vertices.count(iun) == 0) {
                    // Add vertex, attributes and index to arrays
                    vertices->data().push_back(group.second.vertices[iun.index]);
                    if(group.second.texCoords.size() != 0) {
                        texCoords->data().push_back(group.second.texCoords[iun.uv]);
                    }
                    if(group.second.normals.size() != 0) {
                        normals->data().push_back(group.second.normals[iun.normal]);
                    }
                    indices->data().push_back(index);
                    unique_vertices.insert(std::pair<IndexUvNormal, unsigned int>(iun, index));
                    ++index;
                }
                else {
                    // Vertex already exists, find its index
                    indices->data().push_back((*unique_vertices.find(iun)).second);
                }
            };
            f(face.v1);
            f(face.v2);
            f(face.v3);
        }
    }

    return obj;
}

void ObjReader::add_vertex(glm::vec3 const& v) {
    m_currentGroup->vertices.push_back(glm::vec4(v, 1.f));
}

void ObjReader::add_vertex(glm::vec4 const& v) {
    m_currentGroup->vertices.push_back(v);
}

void ObjReader::add_normal(glm::vec3 const& v) {
    m_currentGroup->normals.push_back(v);
}

void ObjReader::add_face(Face const& f) {
    m_currentGroup->faces.push_back(f);
}
