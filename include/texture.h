#ifndef TEXTURE_H
#define TEXTURE_H

#include "gl_core_3_3.h"
#include "debug.h"
#include <string>
#include <vector>

namespace Engine {

// TODO : solve "ownership" issues (i.e. what Texture object owns the texture?)
// Passing pointers around is always error prone, find a better way

/**
  * \class Texture
  * \brief Handle to a texture stored on the GPU.
  */
class Texture {
    friend class FBO;

    public:
    /**
      * \brief Default constructor
      * Construct an empty texture
      */
    Texture();
    /**
      * \brief Destructor
      */
    virtual ~Texture();
    /**
      * \brief Move constructor
      */
    Texture(Texture&& other);
    /**
      * \brief Move-assignment operator
      */
    Texture& operator=(Texture&& other);

    /**
      * \enum Target
      * \brief Enumerate the different texture binding points.
      */
    enum Target : GLenum { Tex2D = GL_TEXTURE_2D };
    /**
      * \enum Filters
      * \brief Enumerate the different texture filters.
      */
    enum Filters : GLenum { Magnification = GL_TEXTURE_MAG_FILTER,
                            Minification  = GL_TEXTURE_MIN_FILTER,
                            Both };
    /**
      * \enum Filter 
      * \brief Enumerate the different texture filtering methods.
      */
    enum Filter : GLenum { Nearest               = GL_NEAREST, 
                           Linear                = GL_LINEAR,
                           BiLinear              = GL_LINEAR_MIPMAP_LINEAR,
                           NearestMipmap_Nearest = GL_NEAREST_MIPMAP_NEAREST,
                           LinearMipmap_Nearest  = GL_LINEAR_MIPMAP_NEAREST,
                           NearestMipmap_Linear  = GL_NEAREST_MIPMAP_LINEAR };

    /**
      * \brief Bind the texture to the specified binding point.
      */
    void bind(Target target = Tex2D) const;
    /**
      * \brief Unbind the texture bound to the specified binding point, if any.
      */
    static void unbind(Target target = Tex2D);
    /**
      * \brief Return a handle to the null texture.
      */
    static Texture noTexture();

    void generateMipmap();

    // TODO : should that be const?
    void filtering(Filters filters, Filter f);
    /**
      * \brief Upload texture data to the GPU
      * \param internalFormat Format in which to store the texture data internally
      * \param format Format in which the texture data is passed
      * \param type Texture data type
      * \param width Texture width
      * \param height Texture height
      * \param data Pointer to the texture data
      */
    void texData(GLint internalFormat, GLenum format, GLenum type, GLint width, GLint height, const void* data);

    /**
      * \brief Return texel data from the texture
      * \param type Pixel type of the returned data
      * \param format Pixel format of the returned data
      * \param size Number of bytes of data to fetch
      * \param level Specify the mipmap image level to fetch data from.
      */
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
