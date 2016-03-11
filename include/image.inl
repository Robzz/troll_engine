template <class T>
Image Image::from_greyscale(std::vector<T> vec, unsigned int width, unsigned int height) {
    assert(vec.size() == (width * height * sizeof(T)));
    Image img(FreeImage_ConvertFromRawBits(reinterpret_cast<unsigned char*>(vec.data()), width, height, width*sizeof(T), sizeof(T)*8, 0xFFFF, 0xFFFF, 0xFFFF));

    FIBITMAP* img8 = FreeImage_ConvertToGreyscale(img.m_image);
    FreeImage_Unload(img.m_image);
    img.m_image = img8;

    return img;
}

template <class Archive>
void Image::save(Archive& ar, const unsigned int version) const {
    FREE_IMAGE_TYPE type = FreeImage_GetImageType(m_image);
    unsigned int width = FreeImage_GetWidth(m_image);
    unsigned int height = FreeImage_GetHeight(m_image);
    unsigned int pitch = FreeImage_GetPitch(m_image);
    unsigned int bpp = FreeImage_GetBPP(m_image);
    unsigned int red_mask = FreeImage_GetRedMask(m_image);
    unsigned int green_mask = FreeImage_GetGreenMask(m_image);
    unsigned int blue_mask = FreeImage_GetBlueMask(m_image);

    std::vector<unsigned char> vec;
    vec.reserve(height * pitch);
    BYTE* bits = FreeImage_GetBits(m_image);
    int n_bits = height * pitch;
    for(int i = 0 ; i != n_bits ; ++i) {
        vec.push_back(bits[i]);
    }

    ar & type & width & height & pitch & bpp & red_mask & green_mask & blue_mask & vec;
}

template <class Archive>
void Image::load(Archive& ar, const unsigned int version) {
    FREE_IMAGE_TYPE type;
    unsigned int width, height, pitch, bpp, red_mask, green_mask, blue_mask;
    std::vector<unsigned char> vec;
    ar & type & width & height & pitch & bpp & red_mask & green_mask & blue_mask & vec;
    m_image = FreeImage_ConvertFromRawBitsEx(true, vec.data(), type, width, height, pitch, bpp, red_mask, green_mask, blue_mask);
}