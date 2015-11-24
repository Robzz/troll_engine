#ifndef SHADER_H
#define SHADER_H

#include <istream>
#include <GL/glew.h>
// #include <GLFW/glfw3.h>

class Shader {
    public:
        Shader(std::istream& src_stream, GLenum shader_type);
        ~Shader();

    private:
        GLuint m_id;
};

#endif
