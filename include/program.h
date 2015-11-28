#ifndef PROGRAM_H
#define PROGRAM_H

#include "shader.h"
#include "gl_core_3_3.h"
#include "debug.h"

#include <sstream>
#include <vector>
#include <typeinfo>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#define TYPE(T) typeid(T).hash_code()

/* Base class for uniforms */
class UniformBase {
    friend class Program;
    public:
        /* Destructor */
        virtual ~UniformBase();

        operator bool() const;

        std::string const& name() const;

    protected:
        virtual void upload() = 0;

        /* Default constructor */
        UniformBase(GLint location, std::string const& name);
        GLint m_location;
        std::string m_name;
        bool m_clean;
};

class Program {
    friend class ProgramBuilder;
    public:
        operator bool() const;
        bool operator !() const;
        std::string info_log() const;

        void use() const;
        static void noProgram();

        GLint getAttributeLocation(std::string) const;
        UniformBase* getUniform(std::string const& name);
        void uploadUniforms();
    
        ~Program();

    private:
        Program(GLuint id, std::vector<UniformBase*> uniforms);
        GLint getUniformLocation(std::string) const;

        GLuint m_id;
        std::vector<UniformBase*> m_uniforms;
};

template <class T>
class Uniform : public UniformBase {
    friend class ProgramBuilder;
    public:
        /* Destructor */
        virtual ~Uniform() {

        }

        /* Modify the uniform value. Does not upload to GPU. */
        void set(T const& value) {
            m_value = value;
            m_clean = false;
        }

    private:
        T m_value;

        /* Default constructor */
        Uniform(GLint location, std::string const& name) :
            UniformBase(location, name),
            m_value()
        { }

        /* Upload uniform to GPU. Program must be in use. */
        virtual void upload() {
            // Do the upload
            if(TYPE(T) == TYPE(glm::vec3)) {
                GLV(glUniform3fv(m_location, 1, glm::value_ptr(m_value)))
            }
            else if(TYPE(T) == TYPE(glm::mat3)) {
                GLV(glUniformMatrix3fv(m_location, 1, GL_FALSE, glm::value_ptr(m_value))) ;
            }
            else if(TYPE(T) == TYPE(glm::mat4)) {
                GLV(glUniformMatrix4fv(m_location, 1, GL_FALSE, glm::value_ptr(m_value)));
            }
            
            m_clean = true;
        }
};

class ProgramBuilder {
    public:
        ProgramBuilder();
        ~ProgramBuilder();

        enum UniformType { vec3, mat3, mat4 };

        /* Attach a shader to the program*/
        ProgramBuilder& attach_shader(Shader&);

        /* Register a uniform */
        ProgramBuilder& with_uniform(std::string const& name, UniformType t);

        /* Link the program and return the Program object */
        Program link();

    private:
        GLuint m_program;
        std::vector<Shader*> m_attachedShaders;
        std::vector<std::pair<std::string, UniformType>> m_uniforms;
};

#undef TYPE

#endif
