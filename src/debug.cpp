#include "debug.h"
#include "mesh.h"

using namespace std;

ostream& operator<<(ostream& s, glm::mat3 const& m) {
    for(int i = 0 ; i != 3 ; ++i) {
        s << "[ ";
        for(int j = 0 ; j != 3 ; ++j) {
            s << m[i][j] << ' ';
        }
        s << ']';
        if(i != 2)
            cout << endl;
    }
    return s;
}

ostream& operator<<(ostream& s, glm::mat4 const& m) {
    for(int i = 0 ; i != 4 ; ++i) {
        s << "[ ";
        for(int j = 0 ; j != 4 ; ++j) {
            s << m[i][j] << ' ';
        }
        s << ']';
        if(i != 3)
            cout << endl;
    }
    return s;
}

ostream& operator<<(ostream& s, glm::vec3 const& v) {
    s << "[ ";
    for(int j = 0 ; j != 3 ; ++j) {
        s << v[j] << ' ';
    }
    s << ']';
    return s;
}

ostream& operator<<(ostream& s, glm::vec4 const& v) {
    s << "[ ";
    for(int j = 0 ; j != 4 ; ++j) {
        s << v[j] << ' ';
    }
    s << ']';
    return s;
}

ostream& operator<<(ostream& s, Engine::Mesh const& m) {
    s << m.name() << " (" << m.numVertices() << ")" << endl;
    bool hasNormals = m.hasNormals(),
         hasColors = m.hasColors(),
         hasUVs = m.hasUVs(),
         isIndexed = m.isIndexed();
    string trueStr = "true", falseStr = "false";
    s << "Has normals: " << (hasNormals ? trueStr : falseStr) << endl;
    s << "Has colors: " << (hasColors ? trueStr : falseStr) << endl;
    s << "Has UVs: " << (hasUVs ? trueStr : falseStr) << endl;
    s << "Is indexed: " << (hasUVs ? trueStr : falseStr);
    if(isIndexed) {
        s << " (has " << m.numFaces() << " faces)";
    }
    return s;
}
