#ifndef TEXT_FREETYPE_H
#define TEXT_FREETYPE_H

#include <string>
#include <ft2build.h>
#include FT_FREETYPE_H

class FreeType;

void check_error(bool e);

class Face {
    friend class FreeType;
    public:
        /* Destructor */
        virtual ~Face();

        long numFaces() const;
        long numGlyphs() const;

        int glyphIndex(char c) const;
        void loadGlyph(int glyphIndex);

        void setCharSize(unsigned int width, unsigned int height, int resolution);
        void setCharSize(unsigned int pixelSize);

    private:
        FT_Face m_face;

        /* Default constructor */
        Face();
};

class FreeType {
    public:
        /* Default constructor */
        FreeType();
        /* Destructor */
        virtual ~FreeType();

        Face loadFace(std::string const& fileName);

        /* No copy or move */
        FreeType(FreeType const& other) = delete;
        FreeType& operator=(FreeType const& other) = delete;
        FreeType(FreeType&& other) = delete;
        FreeType& operator=(FreeType&& other) = delete;

    private:
        FT_Library m_library;
};

#endif // TEXT_FREETYPE_H
