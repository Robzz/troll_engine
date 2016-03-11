#include "gl_core_3_3.h"
#include "image.h"
#include "utility.h"
#include <stdexcept>
#include <fstream>

namespace Engine {

Image::Image(unsigned int width, unsigned int height, unsigned int bpp) :
    m_image(FreeImage_Allocate(static_cast<int>(width), static_cast<int>(height), static_cast<int>(bpp)))
{

}

Image::Image() :
    m_image(nullptr)
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
    m_image(FreeImage_Clone(other))
{ }

Image::~Image() {
    if(m_image)
        FreeImage_Unload(m_image);
}

Image::Image(Image const& other) :
    m_image(FreeImage_Clone(other.m_image))
{ }

Image& Image::operator=(Image const& other) {
    m_image = FreeImage_Clone(other.m_image);
    return *this;
}

unsigned int Image::width() const {
    return FreeImage_GetWidth(m_image);
}

unsigned int Image::height() const {
    return FreeImage_GetHeight(m_image);
}

Image Image::from_rgb(std::vector<unsigned char> vec, int width, int height) {
    assert(vec.size() == (static_cast<unsigned int>(width * height) * 3u));
    Image img(FreeImage_ConvertFromRawBits(vec.data(), width, height, width * 3, 24u, 0xFF000000, 0x00FF0000, 0x0000FF00));
    return img;
}

Texture Image::to_texture() const {
    Texture tex;
    switch(FreeImage_GetBPP(m_image)) {
        case 24:
            unsigned char* buf = new unsigned char[width() * height() * 3];
            FreeImage_ConvertToRawBits(buf, m_image, static_cast<int>(width()) * 3, 24u, 0xFF000000, 0x00FF0000, 0x0000FF00);
            tex.texData(GL_RGB, GL_BGR, GL_UNSIGNED_BYTE, static_cast<int>(width()), static_cast<int>(height()), buf);
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
    return UNREACHABLE(false);
}

} // namespace Engine
