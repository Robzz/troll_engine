#ifndef TEXTURE_H
#define TEXTURE_H

#include "gl_core_3_3.h"
#include <string>

class Texture {
    friend class FBO;

    public:
    /* Default constructor */
    Texture();
    /* Destructor */
    virtual ~Texture();
    /* Move constructor */
    Texture(Texture&& other);
    /* Move-assignment operator */
    Texture& operator=(Texture&& other);

    enum Target { Tex2D = GL_TEXTURE_2D };

    void bind(Target target = Tex2D) const;
    static void unbind(Target target = Tex2D);
    static Texture noTexture();

    static Texture from_image(std::string const& filename);

    void texData(GLint internalFormat, GLenum format, GLenum type, GLint width, GLint height, const void* data);

    private:
    GLuint m_id;

    explicit Texture(GLuint id);
    Texture(Texture const& other);
    void operator=(Texture const& other);
};

#endif
