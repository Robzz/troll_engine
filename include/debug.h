/* Run, you fool! Only black magic happens here. */
#ifndef DEBUG_H
#define DEBUG_H

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glbinding/gl33core/gl.h>
#include <iostream>
#include <vector>
#include <functional>
#include "glm/glm.hpp"

namespace Engine {
    class Mesh;
} // namespace Engine

typedef std::function<void(gl::GLenum source,
                           gl::GLenum type,
                           gl::GLuint id,
                           gl::GLenum severity,
                           gl::GLsizei length,
                           const gl::GLchar* message,
                           const void* userParam)> glDebugCallback;

// Some debug pretty-print functions
std::ostream& operator<<(std::ostream& s, glm::mat3 const& m);
std::ostream& operator<<(std::ostream& s, glm::mat4 const& m);
std::ostream& operator<<(std::ostream& s, glm::vec2 const& v);
std::ostream& operator<<(std::ostream& s, glm::vec3 const& v);
std::ostream& operator<<(std::ostream& s, glm::vec4 const& v);
template<class T>
std::ostream& operator<<(std::ostream& s, std::vector<T> const& v) {
    for(T const& t: v) {
        s << t << std::endl;
    }
    return s;
}
std::ostream& operator<<(std::ostream& s, Engine::Mesh const& m);

/* This macro enables conditional compilation based on debug mode. The inner code will only be
 * executed on debug builds. */
#ifdef DEBUG
#define DBG(code) { code }
#else
#define DBG(code) { }
#endif

#endif
