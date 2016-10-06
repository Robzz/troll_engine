#ifndef TEXTURE_H
#define TEXTURE_H

#include <glbinding/gl33core/gl.h>
#include "debug.h"
#include <string>
#include <vector>

class RGBImage;
class GreyscaleImage;

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

    static Texture fromImage(RGBImage const& img);
    static Texture fromImage(GreyscaleImage const& img);
    static Texture depthTextureFromImage(GreyscaleImage const& img);

    /**
      * \brief Bind the texture to the specified binding point.
      */
    void bind(gl::GLenum target = gl::GL_TEXTURE_2D) const;
    /**
      * \brief Unbind the texture bound to the specified binding point, if any.
      */
    static void unbind(gl::GLenum target = gl::GL_TEXTURE_2D);
    /**
      * \brief Return a handle to the null texture.
      */
    static Texture noTexture();

    void generateMipmap();

    /**
     * @brief Select the texture filtering options.
     * @param minMag Minification of magnification filter
     * @param filter New filtering mode
     */
    void filtering(gl::GLenum minMag, gl::GLenum filter);

    /**
      * \brief Upload texture data to the GPU
      * \param internalFormat Format in which to store the texture data internally
      * \param format Format in which the texture data is passed
      * \param type Texture data type
      * \param width Texture width
      * \param height Texture height
      * \param data Pointer to the texture data
      */
    void texData(gl::GLint internalFormat, gl::GLenum format, gl::GLenum type,
                 gl::GLint width, gl::GLint height, const void* data);

    /**
      * \brief Return texel data from the texture
      * \param type Pixel type of the returned data
      * \param format Pixel format of the returned data
      * \param size Number of bytes of data to fetch
      * \param level Specify the mipmap image level to fetch data from.
      */
    template <class T>
    std::vector<T> get_pixels(gl::GLenum type, gl::GLenum format, size_t size,
                              gl::GLint level = 0) const;

    private:
    gl::GLuint m_id;
    int m_width;
    int m_height;

    explicit Texture(gl::GLuint id);
    Texture(Texture const& other);
    void operator=(Texture const& other);
};

#include "texture.inl"

} // namespace Engine

#endif
