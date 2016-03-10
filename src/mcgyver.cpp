#include "mcgyver.h"
#include "debug.h"
#include "gl_core_3_3.h"
#include <GLFW/glfw3.h>
#include <iostream>

namespace Engine {

void mcgyver_init(int argc, char** argv) {
	if (!glfwInit()) {
        std::cerr << "Error : cannot initialize GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }
    glfwSetErrorCallback([] (int error, const char* description) { std::cerr << description << std::endl; });
}

} // namespace Engine