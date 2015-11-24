#ifndef PROGRAM_H
#define PROGRAM_H

#include "shader.h"

#include <vector>

class Program {
    friend class ProgramBuilder;
    public:
        operator bool() const;
        std::string info_log() const;

        void use() const;
        static void no_program();

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
