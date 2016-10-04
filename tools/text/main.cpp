#include <iostream>
#include <cmath>
#include "freetype.h"

using namespace std;

const int texture_resolution = 4096;
const int resolution = 400;

byte mapFloat01ToByte(float f) {
    return static_cast<int>(f * 255);
}

GreyscaleImage deadReckoning3x3(BinaryImage const& source) {
    std::vector<std::vector<ImageCoords>> p(source.width());
    GreyscaleImage out(source.width(), source.height());

    // Initialization
    for(int x = 0 ; x != source.height() ; ++x) {
        p[x].reserve(source.height());
        for(int y = 0 ; y != source.width() ; ++y) {
            if((y != (source.height() - 1) && source.getPixel(x, y) != source.getPixel(x, y + 1)) ||
               (x != 0 && source.getPixel(x, y) != source.getPixel(x - 1, y)) ||
               (x != (source.width() - 1) && source.getPixel(x, y) != source.getPixel(x + 1, y)) ||
               (y != 0 && source.getPixel(x, y) != source.getPixel(x, y - 1))) {
                p[x][y] = {x, y};
                out.setPixel(x, y, 0);
            }
            else {
                p[x][y] = {-1, -1};
                out.setPixel(x, y, 255);
            }
        }
    }

    float d1 = 1.f, d2 = sqrt(2.f);
    // Forward pass
    for(int y = 1 ; y != source.height() ; ++y) {
        for(int x = 1 ; x != source.width() - 1 ; ++x) {
            int i1, i2;
            if(out.getPixel(x-1, y-1) + d2 < out.getPixel(x, y)) {
                p[x][y] = p[x-1][y-1];
                i1 = x - p[x][y].x, i2 = y - p[x][y].y;
                out.setPixel(x, y, sqrt(i1*i1 + i2*i2));
            }
            if(out.getPixel(x, y-1) + d1 < out.getPixel(x, y)) {
                p[x][y] = p[x][y-1];
                i1 = x - p[x][y].x, i2 = y - p[x][y].y;
                out.setPixel(x, y, sqrt(i1*i1 + i2*i2));
            }
            if(out.getPixel(x+1, y-1) + d2 < out.getPixel(x, y)) {
                p[x][y] = p[x+1][y-1];
                i1 = x - p[x][y].x, i2 = y - p[x][y].y;
                out.setPixel(x, y, sqrt(i1*i1 + i2*i2));
            }
            if(out.getPixel(x-1, y) + d1 < out.getPixel(x, y)) {
                p[x][y] = p[x-1][y];
                i1 = x - p[x][y].x, i2 = y - p[x][y].y;
                out.setPixel(x, y, sqrt(i1*i1 + i2*i2));
            }
        }
    }

    // Backward pass
    for(int y = source.height()- 2 ; y >= 0 ; --y) {
        for(int x = source.width() - 2 ; x != 0 ; --x) {
            int i1, i2;
            if(out.getPixel(x+1, y+1) + d2 < out.getPixel(x, y)) {
                p[x][y] = p[x+1][y+1];
                i1 = x - p[x][y].x, i2 = y - p[x][y].y;
                out.setPixel(x, y, sqrt(i1*i1 + i2*i2));
            }
            if(out.getPixel(x, y+1) + d1 < out.getPixel(x, y)) {
                p[x][y] = p[x][y+1];
                i1 = x - p[x][y].x, i2 = y - p[x][y].y;
                out.setPixel(x, y, sqrt(i1*i1 + i2*i2));
            }
            if(out.getPixel(x-1, y+1) + d2 < out.getPixel(x, y)) {
                p[x][y] = p[x-1][y+1];
                i1 = x - p[x][y].x, i2 = y - p[x][y].y;
                out.setPixel(x, y, sqrt(i1*i1 + i2*i2));
            }
            if(out.getPixel(x+1, y) + d1 < out.getPixel(x, y)) {
                p[x][y] = p[x+1][y];
                i1 = x - p[x][y].x, i2 = y - p[x][y].y;
                out.setPixel(x, y, sqrt(i1*i1 + i2*i2));
            }
        }
    }

    const float spread = 10.f;
    // Final pass
    for(int y = 0 ; y != out.height() ; ++y) {
        for(int x = 0 ; x != out.width() ; ++x) {
            float d = out.getPixel(x, y);
            d = min(max(0.f, d / spread), 1.f);
            if(source.getPixel(x, y))
                out.setPixel(x, y, mapFloat01ToByte(0.5f + d / 2));
            else
                out.setPixel(x, y, mapFloat01ToByte(0.5f - d / 2));
        }
    }

    return out;
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

        auto glyphDistMap = deadReckoning3x3(glyphTile);

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
