template <class T>
Image Image::from_greyscale(std::vector<T> vec, int width, int height) {
    assert(vec.size() == (width * height * sizeof(T)));
    Image img(FreeImage_ConvertFromRawBits(reinterpret_cast<unsigned char*>(vec.data()), width, height, width*sizeof(T), sizeof(T)*8, 0xFFFF, 0xFFFF, 0xFFFF));

    FIBITMAP* img8 = FreeImage_ConvertToGreyscale(img.m_image);
    FreeImage_Unload(img.m_image);
    img.m_image = img8;

    return img;
}
