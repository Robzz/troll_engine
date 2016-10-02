#include <stdexcept>
#include "freetype.h"

using namespace std;

void check_error(bool e) {
    if(e) {
        throw runtime_error("FreeType error");
    }
}

Face::Face() :
    m_face() { }

Face::~Face() { }

long Face::numFaces() const {
    return m_face->num_faces;
}

long Face::numGlyphs() const {
    return m_face->num_glyphs;
}

int Face::glyphIndex(char c) const {
    return FT_Get_Char_Index(m_face, c);
}

void Face::loadGlyph(int glyphIndex) {
    check_error(FT_Load_Glyph(m_face, glyphIndex, 0));
}

void Face::setCharSize(unsigned int width, unsigned int height, int resolution) {
    check_error(FT_Set_Char_Size(m_face, width, height, resolution, resolution));
}

void Face::setCharSize(unsigned int pixelSize) {
    check_error(FT_Set_Pixel_Sizes(m_face, 0, pixelSize));
}

FreeType::FreeType() :
    m_library() {
    check_error(FT_Init_FreeType(&m_library));
}

FreeType::~FreeType() { }

Face FreeType::loadFace(string const& fileName) {
    Face f;
    check_error(FT_New_Face(m_library, fileName.c_str(), 0, &f.m_face));
    return f;
}
