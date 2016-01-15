#ifndef TEXTURE_H
#define TEXTURE_H

#include "gl_core_3_3.h"
#include <string>

class Texture {
    public:
    /* Default constructor */
    Texture();
    /* Destructor */
    virtual ~Texture();
    /* Move constructor */
    Texture(Texture&& other);
    /* Move-assignment operator */
    Texture& operator=(Texture&& other);

    void bind(GLenum target = GL_TEXTURE_2D) const;
    static void unbind(GLenum target = GL_TEXTURE_2D);
    static Texture noTexture();

    static Texture from_image(std::string const& filename);

    void texData(GLint internalFormat, GLint format, GLint type, GLint width, GLint height, const void* data);

    private:
    GLuint m_id;

    Texture(GLuint id);
    Texture(Texture const& other);
    void operator=(Texture const& other);
};

#endif
