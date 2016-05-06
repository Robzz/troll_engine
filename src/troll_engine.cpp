#include "troll_engine.h"
#include "debug.h"
#include "gl_core_3_3.h"
#include <GLFW/glfw3.h>
#include <iostream>

namespace Engine {

void engine_init(int argc, char** argv) {
    if (!glfwInit()) {
        throw std::runtime_error("Cannot initialize GLFW");
    }
    glfwSetErrorCallback([] (int error, const char* description) { std::cerr << description << std::endl; });
}

} // namespace Engine
