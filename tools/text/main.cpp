#include <iostream>
#include <cmath>
#include "freetype.h"

using namespace std;

const int texture_resolution = 4096;
const int resolution = 400;

byte mapFloat01ToByte(float f) {
    return static_cast<int>(f * 255);
}

void remapDistanceFieldRange(GreyscaleImage& img, int spread) {
    for(int y = 0 ; y != img.width() ; ++y) {
        for(int x = 0 ; x != img.height() ; ++x) {
            byte b = img.getPixel(x, y);
            float bf = b;
            bf = (bf - 128.f) / spread;
            bf = clamp<float>(-1.f, 1.f, bf);
            bf = (bf + 1.f) * (255.f / 2.f);
            b = bf;
            img.setPixel(x, y, b);
        }
    }
}

int main(int argc, char** argv) {
    FreeType ft;
    Face f = ft.loadFace("/usr/share/fonts/truetype/msttcorefonts/arial.ttf");
    f.setCharSize(300);

    auto fontMap = BinaryImage(texture_resolution, texture_resolution);
    auto distMap = GreyscaleImage(texture_resolution, texture_resolution);
    for(char c = 33 ; c != 127 ; ++c) {
        auto glyph = f.getCharBitmapBinary(c);
        BinaryImage glyphTile(resolution, resolution);
        int dx = resolution - glyph.width(), dy = resolution - glyph.height();
        glyphTile.blit({dx / 2, dy / 2}, {0, 0, glyph.width(), glyph.height()}, glyph);

        auto glyphDistMap = glyphTile.deadReckoning3x3();
        remapDistanceFieldRange(glyphDistMap, 10);

        int i = c - 32,
            n = texture_resolution / resolution,
            x = i % n, y = i / n;
        fontMap.blit({x * resolution, y * resolution}, {0, 0, resolution, resolution}, glyphTile);
        distMap.blit({x * resolution, y * resolution}, {0, 0, resolution, resolution}, glyphDistMap);

        cout << "Done character " << c << " (" << static_cast<int>(c) << ")" << endl;
    }
    fontMap.save("font.bmp", ImageFormat::Bmp);
    distMap.save("distanceField.png", ImageFormat::Png);
}
