#ifndef IMAGE_H
#define IMAGE_H

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <FreeImage.h>
#include "texture.h"

class Image {
    public:
    /* Default constructor */
    Image(int width, int height, int bpp);
    explicit Image(std::string const& filename);
    /* Destructor */
    virtual ~Image();

    enum class Format { Bmp, BmpRle };

    /* Copy constructor */
    Image(Image const& other);
    /* Assignment operator */
    Image& operator=(Image const& other);

    int width() const;
    int height() const;

    static Image from_rgb(std::vector<unsigned char> vec, int width, int height);
    Texture to_texture() const;

    bool save(std::string const& filename, Format f) const;

    private:
    explicit Image(FIBITMAP* other);
    FIBITMAP* m_image;
};

#endif
