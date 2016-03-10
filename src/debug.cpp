#include "debug.h"
#include <iostream>

/*void register_gl_debug_callback(glDebugCallback const& cb) {
    glDebugMessageCallback(cb.target<GLDEBUGPROC>(), nullptr);
}*/

void APIENTRY gl_cb(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
    std::cout << message << std::endl;
}

void clear_gl_debug_callback() {
    glDebugMessageCallback(nullptr, nullptr);
}

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
