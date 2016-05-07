#ifndef IMAGE_H
#define IMAGE_H

#include "texture.h"
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <FreeImage.h>

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/vector.hpp>

namespace Engine {

class Image {
public:
    friend boost::serialization::access;

    /* Default constructor */
    Image(unsigned int width, unsigned int height, unsigned int bpp);
    explicit Image(std::string const& filename);
    Image();
    /* Destructor */
    virtual ~Image();

    enum class Format { Bmp, BmpRle };

    /* Copy constructor */
    Image(Image const& other);
    /* Assignment operator */
    Image& operator=(Image const& other);

    unsigned int width() const;
    unsigned int height() const;

    static Image from_rgb(std::vector<unsigned char> vec, int width, int height);

    template <class T>
    static Image from_greyscale(std::vector<T> vec, unsigned int width, unsigned int height);
    Texture to_texture() const;
    Texture to_depth_texture() const;

    bool save(std::string const& filename, Format f) const;

protected:
    explicit Image(FIBITMAP* other);
    FIBITMAP* m_image;

private:
    template <class Archive>
    void save(Archive& ar, const unsigned int version) const;

    template <class Archive>
    void load(Archive& ar, const unsigned int version);
    BOOST_SERIALIZATION_SPLIT_MEMBER()
};

#include "image.inl"

} // namespace Engine


#endif
