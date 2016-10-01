/**
 * \file scenegraph.h
 * \brief Scenegraph related classes.
 * \author R.Chavignat
 *
 * This files defines the Scenegraph class and the various node classes.
 *
 */


#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

#include <map>
#include "gl_core_3_3.h"
#include "glm/glm.hpp"

#include "program.h"
#include "vao.h"
#include "vbo.h"
#include "matrixstack.h"
#include "texture.h"
#include "mesh.h"

namespace Engine {

/** \class Node
 *  \brief Base class for nodes in a SceneGraph
           These nodes don't contain geometry themselves, and drawing them
           doesn't render anything unless they have children. Their use is to
           logically organize a scene in the SceneGraph. */
class Node {
    friend class SceneGraph;
    public:
        /** \fn Node
          * \brief Default Node constructor.
          * \param trans   Node transformation matrix.
          * \param name    Node name.
          * \param enabled Enable or disable the Node.
          */
        Node(glm::mat4 const& trans = glm::mat4(1.f), std::string const& name = "", bool enabled = true);
        virtual ~Node();
        /* Add a child node and return its id */
        int addChild(Node* n);
        /* Remove a child with a particular id */
        void removeChildById(int id);
        /* Remove all childs with a given name */
        void removeChildsByName(std::string const&);

        // TODO : use transform class here
        glm::mat4 transform() const;
        void set_transform(glm::mat4 const& m);

    protected:
        /* Do we draw this node and its children? */
        bool m_enabled;
        /* Position relative to the parent node */
        glm::mat4 m_position;
        /* Map of children */
        std::map<int, Node*> m_children;
        /* Tracks the ID given to the next child */
        int m_id;
        /* Node's name */
        std::string m_name;

        /* Return a new node identifier and increment the counter */
        int nextId();
};

/* Root node of a scenegraph. */
class SceneGraph : public Node {
    public:
        SceneGraph();
        ~SceneGraph();
        /* Render the scene */
        void render();
        /* Render a specific node */
        void render(int id);

    private:
        MatrixStack m_matrixStack;

        /* Render a Node from a pointer */
        void render(Node* n);
};

/* Base class for nodes that can be rendered */
class DrawableNode : public Node {
    public:
    /* Default constructor */
    DrawableNode(glm::mat4 const& position, Program* prog, VAO* vao, unsigned int nPrimitives,
                 Texture const* tex = nullptr, GLenum primitiveMode = GL_TRIANGLES);

    /* Render the node */
    virtual void draw(glm::mat4 const& m) = 0;
    void set_texture(Texture const* tex = nullptr);
    void set_program(Program* prog);
    void set_vao(VAO* vao);

    void enable_attribute(std::string const& attr, bool enable = true);

    protected:
    Texture const* m_tex;
    Program* m_program;
    unsigned int m_nPrimitives;
    GLenum m_primitiveMode;
    VAO* m_vao;
};

/* Drawable object with its own geometry, rendered with array rendering. */
class Object : public DrawableNode {
    friend DrawableNode* Mesh::instantiate(glm::mat4 const& position, Program* p, Texture const* tex,
                                           GLenum primitiveMode) const;
    public:
        // Takes ownership of the VAO
        Object(glm::mat4 const& position, Program* p, VAO* vao, unsigned int n_primitives,
               Texture const* tex = nullptr, GLenum primitiveMode = GL_TRIANGLES);
        ~Object();
        virtual void draw(glm::mat4 const& m);

    private:
};

/* Drawable object with its own geometry, rendered with indexed rendering. */
class IndexedObject : public DrawableNode {
    public:
        IndexedObject(glm::mat4 const& position, Program* p, const VBO* ebo, VAO* vao, unsigned int n_indices,
                      Texture const* tex = nullptr, GLenum indexType = GL_UNSIGNED_SHORT,
                      GLenum primitiveMode = GL_TRIANGLES);
        ~IndexedObject();
        virtual void draw(glm::mat4 const& m);

    private:
        const VBO* m_ebo;
        unsigned int m_nIndices;
        GLenum m_indexType;
};

} // namespace Engine

#endif
