#include "image.h"
#include <stdexcept>

Image::Image(int width, int height, int bpp) :
    m_image(FreeImage_Allocate(width, height, bpp))
{ }

Image::Image(std::string const& filename) :
    m_image(nullptr)
{
    FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(filename.c_str());
    if(fif == FIF_UNKNOWN) {
        fif = FreeImage_GetFIFFromFilename(filename.c_str());
        if(fif == FIF_UNKNOWN)
            throw std::runtime_error("Cannot open image");
    }
    m_image = FreeImage_Load(fif, filename.c_str());
}

Image::Image(FIBITMAP* other) :
    m_image(other)
{ }

Image::~Image() { }

Image::Image(Image const& other) :
    m_image(FreeImage_Clone(other.m_image))
{ }

Image& Image::operator=(Image const& other) {
    m_image = FreeImage_Clone(other.m_image);
    return *this;
}

int Image::width() const {
    return FreeImage_GetWidth(m_image);
}

int Image::height() const {
    return FreeImage_GetHeight(m_image);
}

Image Image::from_rgb(std::vector<unsigned char> vec, int width, int height) {
    assert(vec.size() == (width * height * 3));
    Image img(FreeImage_ConvertFromRawBits(vec.data(), width, height, width*3, 24, 0xFF000000, 0x00FF0000, 0x0000FF00));
    return img;
}

Texture Image::to_texture() const {
    Texture tex;
    int bpp = FreeImage_GetBPP(m_image), w = width(), h = height();
    switch(bpp) {
        case 24:
            unsigned char* buf = new unsigned char[w*h*3];
            FreeImage_ConvertToRawBits(buf, m_image, w*3, 24, 0xFF000000, 0x00FF0000, 0x0000FF00);
            tex.texData(GL_RGB, GL_BGR, GL_UNSIGNED_BYTE, FreeImage_GetWidth(m_image), FreeImage_GetHeight(m_image), buf);
            delete[] buf;
            break;
    }
    return tex;
}

bool Image::save(std::string const& filename, Format f) const {
    switch(f) {
        case Format::Bmp:
            return FreeImage_Save(FIF_BMP, m_image, filename.c_str(), BMP_DEFAULT);
        case Format::BmpRle:
            return FreeImage_Save(FIF_BMP, m_image, filename.c_str(), BMP_SAVE_RLE);
    }
}
