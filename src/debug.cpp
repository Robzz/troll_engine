#include "debug.h"

void check_gl_errors() {
    GLenum e;
    while((e = glGetError()) != GL_NO_ERROR) {
        std::cerr << "GL error : " << parse_error(e) << std::endl;
    }
}

const char* parse_error(GLenum err) {
    return \
    err == 0x500 ? "GL_INVALID_ENUM" :
    err == 0x501 ? "GL_INVALID_VALUE" :
    err == 0x502 ? "GL_INVALID_OPERATION" :
    err == 0x503 ? "GL_STACK_OVERFLOW" :
    err == 0x504 ? "GL_STACK_UNDERFLOW" :
    err == 0x505 ? "GL_OUT_OF_MEMORY" :
    err == 0x506 ? "GL_INVALID_FRAMEBUFFER_OPERATION" :
    err == 0x507 ? "GL_CONTEXT_LOST" :
    err == 0x8031 ? "GL_TABLE_TOO_LARGE" :
                    "unknown error code";
}

void wrap_gl_call(const char* call_str, const char* filename, int lineno) {
#ifdef GL_TRACE
    std::cout << call_str << std::endl;
#endif
#ifdef DEBUG
    GLenum e;
    if((e = glGetError()) != GL_NO_ERROR) {
        std::cerr << "GL Error!\n\tCall : " << call_str << "\n\tAt : " << filename << ':' << lineno << "\n\tCause : " << parse_error(e) << std::endl;
    }
#endif
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
