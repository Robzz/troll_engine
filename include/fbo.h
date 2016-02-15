#ifndef FBO_H
#define FBO_H

#include "gl_core_3_3.h"
#include "texture.h"

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

    /* Move constructor */
    FBO(FBO&& other);
    /* Move-assignment operator */
    FBO& operator=(FBO&& other);

    void bind(Target const& t);
    static void bind_default(Target const& t);

    void attach(Target t, Attachment a, Texture::Target tex_t, Texture& tex, GLint layer = 0);
    static void detach(Target t, Attachment a);

    /* Status query */
    static bool is_complete(Target t);
    // TODO : dunno why Status won't work as return type
    static GLenum status(Target t);

    private:
    /* Not copyable. */
    FBO(FBO const& other);
    FBO& operator=(FBO const& other);

    GLuint m_id;
};

#endif
