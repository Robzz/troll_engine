#include "debug.h"
#include <iostream>

std::ostream& operator<<(std::ostream& s, glm::mat3 const& m) {
    for(int i = 0 ; i != 3 ; ++i) {
        s << "[ ";
        for(int j = 0 ; j != 3 ; ++j) {
            s << m[i][j] << ' ';
        }
        s << ']';
        if(i != 2)
            std::cout << std::endl;
    }
    return s;
}

std::ostream& operator<<(std::ostream& s, glm::mat4 const& m) {
    for(int i = 0 ; i != 4 ; ++i) {
        s << "[ ";
        for(int j = 0 ; j != 4 ; ++j) {
            s << m[i][j] << ' ';
        }
        s << ']';
        if(i != 3)
            std::cout << std::endl;
    }
    return s;
}

std::ostream& operator<<(std::ostream& s, glm::vec3 const& v) {
    s << "[ ";
    for(int j = 0 ; j != 3 ; ++j) {
        s << v[j] << ' ';
    }
    s << ']';
    return s;
}

std::ostream& operator<<(std::ostream& s, glm::vec4 const& v) {
    s << "[ ";
    for(int j = 0 ; j != 4 ; ++j) {
        s << v[j] << ' ';
    }
    s << ']';
    return s;
}
