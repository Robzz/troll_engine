#ifndef FBO_H
#define FBO_H

#include "gl_core_3_3.h"
#include "utility.h"
#include "texture.h"

namespace Engine {

class FBO {
    public:
    /* Default constructor */
    FBO();
    /* Destructor */
    virtual ~FBO();

    enum Target : GLenum { Read = GL_READ_FRAMEBUFFER, Draw = GL_DRAW_FRAMEBUFFER, Both = GL_FRAMEBUFFER };
    enum Attachment : GLenum { Color = GL_COLOR_ATTACHMENT0, Depth = GL_DEPTH_ATTACHMENT };
    enum Status : GLenum { Complete = GL_FRAMEBUFFER_COMPLETE,
                           IncompleteAttachment   = GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT,
                           MissingAttachment      = GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT,
                           IncompleteDrawBuffer   = GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER,
                           IncompleteReadBuffer   = GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER,
                           IncompleteMultisample  = GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE,
                           IncompleteLayerTargets = GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS,
                           Unsupported            = GL_FRAMEBUFFER_UNSUPPORTED };
    enum Format : GLenum { Rgb = GL_RGB, Bgr = GL_BGR, Rgba = GL_RGBA, DepthComponent = GL_DEPTH_COMPONENT };
    enum Type : GLenum { Ubyte = GL_UNSIGNED_BYTE, Float = GL_FLOAT, Ushort = GL_UNSIGNED_SHORT };

    /* Move constructor */
    FBO(FBO&& other);
    /* Move-assignment operator */
    FBO& operator=(FBO&& other);

    void bind(Target const& t);
    static void bind_default(Target const& t);

    void attach(Target t, Attachment a, Texture& tex, GLint layer = 0);
    static void detach(Target t, Attachment a);

    /* Status query */
    static bool is_complete(Target t);
    // TODO : dunno why Status won't work as return type
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
