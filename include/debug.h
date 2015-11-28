#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"

#define DEBUG
#define GL_TRACE

void check_gl_errors();
const char* parse_error(GLenum err);

void wrap_gl_call(const char* call_str, const char* filename, int lineno);

template <class T>
T wrap_gl_call(T val, const char* call_str, const char* filename, int lineno) {
    wrap_gl_call(call_str, filename, lineno);
    return val;
}

std::ostream& operator<<(std::ostream& s, glm::mat3 const& m);
std::ostream& operator<<(std::ostream& s, glm::mat4 const& m);
std::ostream& operator<<(std::ostream& s, glm::vec3 const& v);

#define GL(c) (wrap_gl_call(c, #c, __FILE__, __LINE__))
#define GLV(c) { c; wrap_gl_call(#c, __FILE__, __LINE__); }

#endif
