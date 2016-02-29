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
#include "glm/glm.hpp"
#include "gl_core_3_3.h"

#include "matrixstack.h"
#include "texture.h"

namespace Engine {

class Program;
class VBO;
class VAO;

/** \class Node
 *  \brief Base class for nodes in a SceneGraph 
           These nodes don't contain geometry themselves, and drawing them
           doesn't render anything unless they have children. They are used to
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
        /* Add a child node and return it's id */
        int addChild(Node* n);
        /* Remove a child with a particular id */
        void removeChildById(int id);
        /* Remove all childs with a given name */
        void removeChildsByName(std::string const&);

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
    DrawableNode(glm::mat4 const& position, Texture const& tex);
    
    /* Render the node */
    virtual void draw(glm::mat4 const& m) = 0;

    protected:
    Texture const& m_tex;
};

/* Drawable object with its own geometry, rendered with array rendering. */
class Object : public DrawableNode {
    public:
        Object(glm::mat4 const& position, Program* p, VAO* vao, int n_primitives,
               Texture const& tex = Texture::noTexture(), GLenum primitiveMode = GL_TRIANGLES);
        ~Object();
        virtual void draw(glm::mat4 const& m);

        void set_program(Program* prog);
        void set_vao(VAO* vao);

    private:
        int m_n_primitives;
        GLenum m_primitiveMode;
        Program* m_program;
        VAO* m_vao;
};

/* Drawable object with its own geometry, rendered with indexed rendering. */
class IndexedObject : public DrawableNode {
    public:
        IndexedObject(glm::mat4 const& position, Program* p, VBO* ebo, VAO* vao, int nVertices,
                      Texture const& tex = Texture::noTexture(), GLenum indexType = GL_UNSIGNED_SHORT,
                      GLenum primitiveMode = GL_TRIANGLES);
        ~IndexedObject();
        virtual void draw(glm::mat4 const& m);

        void set_program(Program* prog);
        void set_vao(VAO* vao);

    private:
        Program* m_program;
        VBO* m_ebo;
        VAO* m_vao;
        int m_nVertices;
        GLenum m_indexType;
        GLenum m_primitiveMode;
};

} // namespace Engine

#endif
