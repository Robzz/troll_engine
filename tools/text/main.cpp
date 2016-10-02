#include <iostream>
#include "freetype.h"

using namespace std;

int main(int argc, char** argv) {
    FreeType ft;
    Face f = ft.loadFace("/usr/share/fonts/truetype/msttcorefonts/arial.ttf");
    f.setCharSize(64);
    cout << "Face has " << f.numGlyphs() << " glyphs" << endl;
    f.loadGlyph(f.glyphIndex('a'));
}
