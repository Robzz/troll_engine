#include <iostream>
#include <cmath>
#include <yaml-cpp/yaml.h>
#include <fstream>
#include "freetype.h"
#include "bsptree2d.h"

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

int main(int, char**) {
    FreeType ft;
    Face f = ft.loadFace("/usr/share/fonts/truetype/msttcorefonts/arial.ttf");
    f.setCharSize(resolution);

    auto fontMap = BinaryImage(texture_resolution, texture_resolution);
    auto distMap = GreyscaleImage(texture_resolution, texture_resolution);
    BspTree2D bsp({0, 0, texture_resolution, texture_resolution});
    YAML::Emitter out;
    out << YAML::BeginMap;
    for(char c = 33 ; c != 127 ; ++c) {
        auto glyph = f.getCharBitmapBinary(c);
        BinaryImage glyphTile(resolution, resolution);
        int dx = resolution - glyph.width(), dy = resolution - glyph.height();
        glyphTile.blit({dx / 2, dy / 2}, {0, 0, glyph.width(), glyph.height()}, glyph);

        auto glyphDistMap = glyphTile.deadReckoning3x3();
        remapDistanceFieldRange(glyphDistMap, 10);
        auto aabb = glyphDistMap.getAABB(0);
        try {
            auto& n = bsp.root()->fit(aabb.width, aabb.height);
            n.setChar(c);
            auto r = n.getRect();
            fontMap.blit({r.x, r.y}, {aabb.x, aabb.y, aabb.width, aabb.height}, glyphTile);
            distMap.blit({r.x, r.y}, {aabb.x, aabb.y, aabb.width, aabb.height}, glyphDistMap);
            cout << "Fit character " << c << " at " << r << endl;
            out << YAML::Key << to_string(static_cast<int>(c))
                << YAML::Value << YAML::BeginSeq
                    << r.x
                    << r.y
                    << r.width
                    << r.height
                << YAML::EndSeq;
        }
        catch(runtime_error& e) {
            cout << "Couldn't fit character " << c << endl;
            cout << "Error: " << e.what() << endl;
        }
    }
    out << YAML::EndMap;
    ofstream outFile("font.yaml", ios_base::out | ios_base::trunc);
    outFile << out.c_str();
    outFile.close();
    fontMap.save("font.bmp", ImageFormat::Bmp);
    distMap.save("distanceField.png", ImageFormat::Png);
}
