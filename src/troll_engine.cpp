#include <iostream>
#include "gl_core_3_3.h"
#include <GLFW/glfw3.h>
#include "troll_engine.h"
#include "window.h"

namespace Engine {

unsigned int TrollEngine::s_instanceCount = 0;

TrollEngine::TrollEngine() {
    ++s_instanceCount;
    if (!glfwInit()) {
        throw std::runtime_error("Cannot initialize GLFW");
    }
    glfwSetErrorCallback([] (int error, const char* description) { std::cerr << description << std::endl; });
    FreeImage_SetOutputMessage(&freeimage_error_callback);
}

TrollEngine::~TrollEngine() {
    --s_instanceCount;
    if(s_instanceCount == 0) {
        glfwTerminate();
    }
}

Window* TrollEngine::current_window() const { return Window::findWindowFromGlfwHandle(glfwGetCurrentContext()); }

void TrollEngine::freeimage_error_callback(FREE_IMAGE_FORMAT fif, const char* message) {
    if(fif != FIF_UNKNOWN) {
        std::cerr << "Format : " << FreeImage_GetFormatFromFIF(fif) << std::endl;
    }
    std::cerr << (message) << std::endl;
}

} // namespace Engine
