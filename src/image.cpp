#include "gl_core_3_3.h"
#include "image.h"
#include "utility.h"
#include <stdexcept>
#include <fstream>
#include <limits>

namespace Engine {

Image::Image(unsigned int width, unsigned int height, unsigned int bpp) :
    m_image(FreeImage_Allocate(static_cast<int>(width), static_cast<int>(height), static_cast<int>(bpp)))
{

}

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

Image Image::from_rgb(std::vector<unsigned char> vec, int width, int height, bool flip) {
    assert(vec.size() == (static_cast<unsigned int>(width * height) * 3u));
    Image img(FreeImage_ConvertFromRawBits(vec.data(), width, height, width * 3, 24u, 0xFF000000, 0x00FF0000, 0x0000FF00, flip));
    return img;
}

Image Image::from_greyscale(std::vector<unsigned short> vec, int width, int height, bool flip) {
    assert(vec.size() == static_cast<size_t>(width * height * 2));
    FIBITMAP* img = FreeImage_AllocateT(FIT_UINT16, width, height, 16);
    int scanline_iter = flip? height-1 : 0,
        scanline_iter_incr = flip? -1 : 1;
    for(int i = 0 ; i != height ; ++i, scanline_iter += scanline_iter_incr) {
        unsigned short* scanline = reinterpret_cast<unsigned short*>(FreeImage_GetScanLine(img, i));
        for(int j = 0 ; j != width ; ++j)
            scanline[j] = vec[static_cast<size_t>(scanline_iter*width + j)];
    }

    Image i(img);

    return i;
}

Texture* Image::to_texture() const {
    Texture* tex = new Texture();
    unsigned char* buf = nullptr;
    switch(FreeImage_GetBPP(m_image)) {
        case 24:
            buf = reinterpret_cast<unsigned char*>(FreeImage_GetBits(m_image));
            tex->texData(GL_RGB, GL_BGR, GL_UNSIGNED_BYTE, static_cast<int>(width()), static_cast<int>(height()), buf);
            break;
        default:
            throw std::runtime_error("Image bpp is not 24 bits");
            break;
    }
    tex->generateMipmap();
    return tex;
}

Texture* Image::to_depth_texture() const {
    Texture* tex = new Texture();
    if(FreeImage_GetBPP(m_image) != 16 || FreeImage_GetImageType(m_image) != FIT_UINT16) {
        throw std::runtime_error("Incompatible bit depth (must be 16bit greyscale)");
    }
    std::vector<float> v;
    for(int i = 0 ; i != static_cast<int>(height()) ; ++i) {
        unsigned short* scanline = reinterpret_cast<unsigned short*>(FreeImage_GetScanLine(m_image, i));
        for(unsigned int j = 0 ; j != width() ; ++j) {
            float f = static_cast<float>(scanline[j]) / std::numeric_limits<unsigned short>::max();
            v.push_back(f);
        }
    }
    GLint alignment, swap_bytes;
    glGetIntegerv(GL_UNPACK_ALIGNMENT, &alignment);
    glGetIntegerv(GL_UNPACK_SWAP_BYTES, &swap_bytes);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
    //glPixelStorei(GL_UNPACK_SWAP_BYTES, 1);
    tex->texData(GL_DEPTH_COMPONENT16, GL_DEPTH_COMPONENT, GL_FLOAT, static_cast<int>(width()), static_cast<int>(height()), v.data());
    glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    glPixelStorei(GL_UNPACK_SWAP_BYTES, swap_bytes);
    return tex;
}

bool Image::save(std::string const& filename, Format f) const {
    switch(f) {
        case Format::Bmp:
            return FreeImage_Save(FIF_BMP, m_image, filename.c_str(), BMP_DEFAULT);
        case Format::BmpRle:
            return FreeImage_Save(FIF_BMP, m_image, filename.c_str(), BMP_SAVE_RLE);
        case Format::Png:
            return FreeImage_Save(FIF_PNG, m_image, filename.c_str(), PNG_DEFAULT);
    }
    return UNREACHABLE(false);
}

} // namespace Engine
