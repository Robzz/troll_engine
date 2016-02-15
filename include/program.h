#ifndef PROGRAM_H
#define PROGRAM_H

#include "shader.h"
#include "gl_core_3_3.h"
#include "debug.h"

#include <sstream>
#include <vector>
#include <type_traits>
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

        // Check if the uniform is valid
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

// Class for GPU programs
class Program {
    friend class ProgramBuilder;
    public:
        Program(Program&& other);
        Program& operator=(Program&& other);

        // Check if the program is valid (i.e. successfully linked)
        operator bool() const;
        // Check if the program is invalid
        bool operator !() const;
        // Return some information about the program.
        // TODO : return information about the uniforms too (e.g. found/not found)
        std::string info_log() const;

        // Set as the program currently used for rendering
        void use() const;
        // Disable programs for rendering
        static void noProgram();

        // Return the OpenGL location of an attribute
        GLint getAttributeLocation(std::string) const;
        // Return the Uniform object associated with an uniform
        UniformBase* getUniform(std::string const& name);
        // Upload uniforms to GPU
        void uploadUniforms();

        bool is_current() const;
        static const Program* current();
    
        ~Program();

    private:
        Program(GLuint id, std::vector<UniformBase*> uniforms);
        
        // Not copyable
        Program(Program const& other);
        Program& operator=(Program const& other);

        GLint getUniformLocation(std::string) const;

        GLuint m_id;
        std::vector<UniformBase*> m_uniforms;
        static const Program* s_current;
};

// Class for Uniform objects
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
            if(!m_clean) {
                if(TYPE(T) == TYPE(glm::vec3)) {
                    GLV(glUniform3fv(m_location, 1, glm::value_ptr(m_value)));
                }
                else if(TYPE(T) == TYPE(glm::mat3)) {
                    GLV(glUniformMatrix3fv(m_location, 1, GL_FALSE, glm::value_ptr(m_value))) ;
                }
                else if(TYPE(T) == TYPE(glm::mat4)) {
                    GLV(glUniformMatrix4fv(m_location, 1, GL_FALSE, glm::value_ptr(m_value)));
                }
                
                m_clean = true;
            }
        }
};

template <>
class Uniform<int> : public UniformBase {
    friend class ProgramBuilder;
    public:
        virtual ~Uniform() { }

        void set(int value) {
            m_value = value;
            m_clean = false;
        }

    private:
        int m_value;

        Uniform(GLint location, std::string const& name) :
            UniformBase(location, name),
            m_value()
        { }

        virtual void upload() {
            if(!m_clean) {
                GLV(glUniform1i(m_location, m_value));
                m_clean = true;
            }
        }
};

template <>
class Uniform<float> : public UniformBase {
    friend class ProgramBuilder;
    public:
        virtual ~Uniform() { }

        void set(float value) {
            m_value = value;
            m_clean = false;
        }

    private:
        float m_value;

        Uniform(GLint location, std::string const& name) :
            UniformBase(location, name),
            m_value()
        { }

        virtual void upload() {
            if(!m_clean) {
                GLV(glUniform1f(m_location, m_value));
                m_clean = true;
            }
        }
};

// Use this class to build Program objects
class ProgramBuilder {
    public:
        ProgramBuilder();
        ~ProgramBuilder();

        enum UniformType { vec3, mat3, mat4, int_, float_ };

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
