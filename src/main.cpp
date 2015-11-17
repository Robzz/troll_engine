#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "window.h"

void error_callback(int error, const char* description) {
    std::cerr << description << std::endl;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}


void init_libs(int argc, char** argv) {
    // Initialize GLEW and GLFW
    if(!glewInit()) {
        std::cerr << "Error : cannot initialize GLEW" << std::endl;
        exit(EXIT_FAILURE);
    }

    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        std::cerr << "Error : cannot initialize GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }
    
    glfwSwapInterval(1);
}


int main(int argc, char** argv) {
    init_libs(argc, argv);

    Window window(1280, 720);
    if (!window) {
        std::cerr << "Error : cannot create window" << std::endl;
        glfwTerminate();
        return -1;
    }
    window.makeCurrent();
    glfwSetKeyCallback(window.m_w, key_callback);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window.m_w))
    {
        /* Render here */

        /* Swap front and back buffers */
        glfwSwapBuffers(window.m_w);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}
