#ifndef IMAGE_H
#define IMAGE_H

#include "texture.h"
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <FreeImage.h>
#include "texture.h"

namespace Engine {

/**
  * \class Image
  * \brief Represents an image in memory.
  */
class Image {
    public:
    /**
      * \brief Construct an empty image of specified dimensions and bit depth.
      * \param width Image width
      * \param height Image height 
      * \param bpp Image Depth, in bits per pixel 
      */
    Image(unsigned int width, unsigned int height, unsigned int bpp);

    Image() = delete;

    /**
      * \brief Construct an image from a file.
      * In theory, any format supporteed by the FreeImage library should work.
      */
    explicit Image(std::string const& filename);
    /* Destructor */
    virtual ~Image();

    /**
      * \enum Format
      * \brief Represent the supported image file formats.
      */
    enum class Format {
        /** Bitmap format */
        Bmp,
        /** Bitmap format, with run length encoding */
        BmpRle,
        /** PNG format */
        Png };

    /**
      * \brief Construct a copy of an Image.
      */
    Image(Image const& other);
    /**
      * \brief Image assignment operator.
      */
    Image& operator=(Image const& other);

    /**
      * \brief Return the width of the image.
      */
    unsigned int width() const;

    /**
      * \brief Return the height of the image.
      */
    unsigned int height() const;

    /**
      * \brief Create an image from an array of 8-bit RGB values.
      * \param vec RGB data byte array
      * \param width Image width
      * \param height Image height
      * \param flip Flip the image vertically
      */
    static Image from_rgb(std::vector<unsigned char> vec, int width, int height, bool flip = false);
    static Image from_greyscale(std::vector<unsigned short> vec, int width, int height, bool flip = false);

    /**
      * \brief Convert the image to a texture that can be used for rendering.
      */
    Texture* to_texture() const;
    Texture* to_depth_texture() const;

    /**
      * \brief Save an image to the disk.
      * \param filename File name
      * \param f Image format
      */
    bool save(std::string const& filename, Format f) const;

    private:
    explicit Image(FIBITMAP* other);
    FIBITMAP* m_image;
};

} // namespace Engine


#endif
