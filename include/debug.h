/* Run, you fool! Only black magic happens here. */
#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>
#include <vector>
#include <functional>
#include "gl_core_3_3.h"
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"

typedef std::function<void(GLenum source,
                           GLenum type,
                           GLuint id,
                           GLenum severity,
                           GLsizei length,
                           const GLchar* message,
                           const void* userParam)> glDebugCallback;

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

/* This macro enables conditional compilation based on debug mode. The inner code will only be
 * executed on debug builds. */
#ifdef DEBUG
#define DBG(code) { code }
#else
#define DBG(code) { }
#endif

#endif
