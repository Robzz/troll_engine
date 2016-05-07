#ifndef TEXTURE_H
#define TEXTURE_H

#include "gl_core_3_3.h"
#include "debug.h"
#include <string>
#include <vector>

namespace Engine {

// TODO : solve "ownership" issues (i.e. what Texture object owns the texture?)
// Passing pointers around is always error prone, find a better way

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

    enum Target : GLenum { Tex2D = GL_TEXTURE_2D };
    enum Filters : GLenum { Magnification = GL_TEXTURE_MAG_FILTER,
                            Minification  = GL_TEXTURE_MIN_FILTER,
                            Both };
    enum Filter : GLenum { Nearest               = GL_NEAREST, 
                           Linear                = GL_LINEAR,
                           BiLinear              = GL_LINEAR_MIPMAP_LINEAR,
                           NearestMipmap_Nearest = GL_NEAREST_MIPMAP_NEAREST,
                           LinearMipmap_Nearest  = GL_LINEAR_MIPMAP_NEAREST,
                           NearestMipmap_Linear  = GL_NEAREST_MIPMAP_LINEAR };

    void bind(Target target = Tex2D) const;
    static void unbind(Target target = Tex2D);
    static Texture noTexture();

    // TODO : should that be const?
    void filtering(Filters filters, Filter f);
    void texData(GLint internalFormat, GLenum format, GLenum type, GLint width, GLint height, const void* data);

    template <class T>
    std::vector<T> get_pixels(GLenum type, GLenum format, size_t size, GLint level = 0) const;

    private:
    GLuint m_id;
    int m_width;
    int m_height;

    explicit Texture(GLuint id);
    Texture(Texture const& other);
    void operator=(Texture const& other);
};

#include "texture.inl"

} // namespace Engine

#endif
