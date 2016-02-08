/* Run, you fool! Only black magic happens here. */
#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>
#include <vector>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"

void check_gl_errors();
const char* parse_error(GLenum err);

void wrap_gl_call(const char* call_str, const char* filename, int lineno);

template <class T>
T wrap_gl_call(T val, const char* call_str, const char* filename, int lineno) {
    wrap_gl_call(call_str, filename, lineno);
    return val;
}

// Some debug pretty-print functions
std::ostream& operator<<(std::ostream& s, glm::mat3 const& m);
std::ostream& operator<<(std::ostream& s, glm::mat4 const& m);
std::ostream& operator<<(std::ostream& s, glm::vec3 const& v);
std::ostream& operator<<(std::ostream& s, glm::vec4 const& v);
template<class T>
std::ostream& operator<<(std::ostream& s, std::vector<T> const& v) {
    for(T const& t: v) {
        s << t << std::endl;
    }
    return s;
}

/* These two macros are designed to wrap calls to OpenGL functions.
 * If GL_TRACE is defined, they will log the OpenGL calls to the standard error stream.
 * If DEBUG is defined, they will check for OpenGL errors after each call. */
#define GL(c) (wrap_gl_call(c, #c, __FILE__, __LINE__))
#define GLV(c) { c; wrap_gl_call(#c, __FILE__, __LINE__); }

/* This macro enables conditional compilation based on debug mode. The inner code will only be
 * executed on debug builds. */
#ifdef DEBUG
#define DBG(code) { code }
#else
#define DBG(code) { }
#endif

#endif
