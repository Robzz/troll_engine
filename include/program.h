#ifndef PROGRAM_H
#define PROGRAM_H

#include "shader.h"
#include "gl_core_3_3.h"

#include <vector>
#include <glm/glm.hpp>

class Program {
    friend class ProgramBuilder;
    public:
        operator bool() const;
        bool operator !() const;
        std::string info_log() const;

        void use() const;
        static void noProgram();

        GLint getAttributeLocation(std::string) const;
        GLint getUniformLocation(std::string) const;
    
        void sendUniform(GLint location, glm::mat4 const&);
        void sendUniform(GLint location, glm::mat3 const&);
        void sendUniform(GLint location, glm::vec3 const&);

        ~Program();

    private:
        Program(GLuint id);

        GLuint m_id;
};

class ProgramBuilder {
    public:
        ProgramBuilder();
        ~ProgramBuilder();

        ProgramBuilder& attach_shader(Shader&);
        Program link();

    private:
        GLuint m_program;
        std::vector<Shader*> m_attachedShaders;
};

#endif
