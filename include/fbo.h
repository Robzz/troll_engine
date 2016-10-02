/**
  * \file fbo.h
  * \brief Defines the FBO class.
  * \author R.Chavignat
  */
#ifndef FBO_H
#define FBO_H

#include <glbinding/gl33core/gl.h>
#include "utility.h"
#include "texture.h"

namespace Engine {

/**
  * \class FBO
  * \brief Represents an Opengl::GL framebuffer object.
  */
class FBO {
    public:
    /**
      * \brief Default constructor.
      */
    FBO();

    /**
      * \brief Destructor.
      */
    virtual ~FBO();

    /* Move constructor */
    FBO(FBO&& other);
    /* Move-assignment operator */
    FBO& operator=(FBO&& other);

    /**
      * \fn bind
      * \brief Bind the FBO to the specified target.
      * \param t gl::GLenum to bind the FBO to.
      */
    void bind(gl::GLenum const& t);

    /**
      * \fn bind_default
      * \brief Bind the default FBO to the specified target.
      * \param t gl::GLenum to bind the FBO to.
      */
    static void bind_default(gl::GLenum const& t);

    /**
      * \fn attach
      * \brief Attach a texture to an attachment point.
      * \param t Framebuffer target
      * \param a gl::GLenum point
      * \param tex Texture to attach
      * \param layer Mipmap level of the texture to attach
      */
    void attach(gl::GLenum t, gl::GLenum a, Texture& tex, gl::GLint layer = 0);

    /**
      * \fn detach
      * \brief Detach a texture from an attachment point.
      * \param t gl::GLenum FBO
      * \param a gl::GLenum point to unbind the texture from
      */
    static void detach(gl::GLenum t, gl::GLenum a);

    /**
      * \brief Check if the FBO is framebuffer-complete.
      * \param t gl::GLenum to check
      * \return true if the FBO is framebuffer-complete, false otherwise.
      */
    static bool is_complete(gl::GLenum t);

    // TODO : dunno why Status won't work as return type
    /**
      * \brief Return the FBOs status
      * \param t gl::GLenum to check
      * \return FBO status
      */
    static gl::GLenum status(gl::GLenum t);

    template <class T>
    static std::vector<T> readPixels(gl::GLenum f, gl::GLenum t, gl::GLsizei width,
                                     gl::GLsizei height, int x = 0, int y = 0);

    private:
    /* Not copyable. */
    FBO(FBO const& other);
    FBO& operator=(FBO const& other);

    gl::GLuint m_id;
};

#include "fbo.inl"

} // namespace Engine

#endif
