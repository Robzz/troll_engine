#include <iostream>
#include "freetype.h"

using namespace std;

const int texture_resolution = 4096;
const int char_width = 400;

void put_glyph(char c, GreyscaleImage& fontMap, Face& font) {
    int i = c - 16;
    int w = texture_resolution / char_width,
        x = (i % w) * char_width,
        y = (i / w) * char_width;

    GreyscaleImage glyph = font.getCharBitmapBinary(c);
    for(int px = 0 ; px != char_width ; ++px) {
        for(int py = 0 ; py != char_width ; ++py) {
            fontMap.setPixel(x + px, y + py, glyph.getPixel(x, y));
        }
    }
}

int main(int argc, char** argv) {
    FreeType ft;
    Face f = ft.loadFace("/usr/share/fonts/truetype/msttcorefonts/arial.ttf");
    f.setCharSize(char_width);

    auto fontMap = GreyscaleImage(texture_resolution, texture_resolution);
    for(char c = 16 ; c != 127 ; ++c) {
        put_glyph(c, fontMap, f);
    }
    fontMap.save("font.png", ImageFormat::Png);
}
