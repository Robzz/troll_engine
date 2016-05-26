/**
  * \file fbo.h
  * \brief Defines the FBO class.
  * \author R.Chavignat
  */
#ifndef FBO_H
#define FBO_H

#include "gl_core_3_3.h"
#include "utility.h"
#include "texture.h"

namespace Engine {

/**
  * \class FBO
  * \brief Represents an OpenGL framebuffer object.
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

    // TODO : can i use strongly typed enums here?
    /**
      * \enum Target
      * \brief Targets the framebuffer can be bound to.
      */
    enum Target : GLenum { Read = GL_READ_FRAMEBUFFER, Draw = GL_DRAW_FRAMEBUFFER, Both = GL_FRAMEBUFFER };

    /**
      * \enum Attachment
      * \brief Framebuffer attachment types.
      */
    enum Attachment : GLenum { Color = GL_COLOR_ATTACHMENT0, Depth = GL_DEPTH_ATTACHMENT };

    /**
      * \enum Status
      * \brief Framebuffer status.
      */
    enum Status : GLenum { Complete = GL_FRAMEBUFFER_COMPLETE,
                           IncompleteAttachment   = GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT,
                           MissingAttachment      = GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT,
                           IncompleteDrawBuffer   = GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER,
                           IncompleteReadBuffer   = GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER,
                           IncompleteMultisample  = GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE,
                           IncompleteLayerTargets = GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS,
                           Unsupported            = GL_FRAMEBUFFER_UNSUPPORTED };

    /**
      * \enum Format
      * \brief Framebuffer image formats.
      */
    enum Format : GLenum { Rgb = GL_RGB, Bgr = GL_BGR, Rgba = GL_RGBA, DepthComponent = GL_DEPTH_COMPONENT };

    /**
      * \enum Type
      * \brief GL Types.
      */
    enum Type : GLenum { Ubyte = GL_UNSIGNED_BYTE, Float = GL_FLOAT, Ushort = GL_UNSIGNED_SHORT };

    /* Move constructor */
    FBO(FBO&& other);
    /* Move-assignment operator */
    FBO& operator=(FBO&& other);

    /**
      * \fn bind
      * \brief Bind the FBO to the specified target.
      * \param t Target to bind the FBO to.
      */
    void bind(Target const& t);

    /**
      * \fn bind_default
      * \brief Bind the default FBO to the specified target.
      * \param t Target to bind the FBO to.
      */
    static void bind_default(Target const& t);

    /**
      * \fn attach
      * \brief Attach a texture to an attachment point.
      * \param t Framebuffer target
      * \param a Attachment point
      * \param tex Texture to attach
      * \param layer Mipmap level of the texture to attach
      */
    void attach(Target t, Attachment a, Texture& tex, GLint layer = 0);

    /**
      * \fn detach
      * \brief Detach a texture from an attachment point.
      * \param t Target FBO
      * \param a Attachment point to unbind the texture from
      */
    static void detach(Target t, Attachment a);

    /**
      * \brief Check if the FBO is framebuffer-complete.
      * \param t Target to check
      * \return true if the FBO is framebuffer-complete, false otherwise.
      */
    static bool is_complete(Target t);
    
    // TODO : dunno why Status won't work as return type
    /**
      * \brief Return the FBOs status
      * \param t Target to check
      * \return FBO status
      */
    static GLenum status(Target t);

    template <class T>
    static std::vector<T> readPixels(Format f, Type t, GLsizei width, GLsizei height, int x = 0, int y = 0);

    private:
    /* Not copyable. */
    FBO(FBO const& other);
    FBO& operator=(FBO const& other);

    GLuint m_id;
};

#include "fbo.inl"

} // namespace Engine

#endif
