/**
  * \file program.h
  * \brief Defines classes related to GPU programs
  * \author R.Chavignat
  */

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

/**
  * \class UniformBase
  * \brief Abstract base class for GPU program uniforms.
  */
class UniformBase {
    friend class Program;
    public:
        /**
          * \brief Destructor
          */
        virtual ~UniformBase();

        /**
          * \brief Check if the uniform is valid
          * \return true if the uniform is valid, false otherwise.
          */
        operator bool() const;

        /**
          * \brief Return the uniforms name.
          */
        std::string const& name() const;

    protected:
        /**
          * \brief Upload the uniform to the GPU.
          */
        virtual void upload() = 0;

        /**
          * \brief Default constructor
          * \param location Uniform location
          * \param name Uniform name
          */
        UniformBase(GLint location, std::string const& name);

        /** \brief OpenGL uniform location */
        GLint m_location;

        /** \brief Uniform name */
        std::string m_name;

        /** \brief Was uniform value modified since last GPU upload */
        bool m_clean;
};

/**
  * \class Program
  * \brief Class for GPU programs
  */
class Program {
    friend class ProgramBuilder;
    public:
        /**
          * \brief Move constructor.
          */
        Program(Program&& other);

        /**
          * \brief Move-assignment operator.
          */
        Program& operator=(Program&& other);

        /**
          * \brief Check if the program is valid
          * \return true if the program is valid, false otherwise.
          */
        operator bool() const;

        /**
          * \brief Check if the program is invalid
          * \return true if the program is invalid, false otherwise.
          */
        bool operator !() const;

        // TODO : return information about the uniforms too (e.g. found/not found)
        /**
          * \brief Return information about the program.
          */
        std::string info_log() const;

        /**
          * \brief Set as the currently active program.
          */
        void use() const;

        /**
          * \brief Unbind the currently bound program, if any.
          */
        static void noProgram();

        /**
          * \brief Return the OpenGL location of an attribute
          */
        GLint getAttributeLocation(std::string) const;
        
        /**
          * \brief Return the Uniform object associated with a uniform
          */
        UniformBase* getUniform(std::string const& name);
        
        /**
          * \brief Upload uniforms to GPU
          */
        void uploadUniforms();

        /**
          * \brief Check if the program is currently in use
          */
        bool is_current() const;

        /**
          * \brief Return the currently bound program.
          */
        static const Program* current();
    
        /**
          * \brief Destructor
          */
        ~Program();

    private:
        /**
          * \brief Constructor
          * \param id OpenGL id of the program.
          * \param uniforms Uniforms used by the program
          */
        Program(GLuint id, std::vector<UniformBase*> uniforms);
        
        // Not copyable
        Program(Program const& other);
        Program& operator=(Program const& other);

        /**
          * \brief Return the location of a uniform
          * \param uni Name of the uniform whose location to query.
          */
        GLint getUniformLocation(std::string const& uni) const;

        GLuint m_id;
        std::vector<UniformBase*> m_uniforms;
        static const Program* s_current;
};

/**
  * \class Uniform
  * \brief Class for uniform objects
  */
template <class T>
class Uniform : public UniformBase {
    friend class ProgramBuilder;
    public:
        /**
          * \brief Destructor
          */
        virtual ~Uniform() {

        }

        /**
          * \brief Set the uniform value, do not upload to the GPU.
          * \param value New value.
          */
        void set(T const& value) {
            m_value = value;
            m_clean = false;
        }

    private:
        T m_value;

        /**
          * \brief Default constructor
          * \param location OpenGL uniform location
          * \param name Uniform name
          */
        Uniform(GLint location, std::string const& name) :
            UniformBase(location, name),
            m_value()
        { }

        /**
          * \brief Upload uniform to GPU. Program must be in use.
          */
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

/**
  * \class Uniform<int>
  * \brief Specialization for uniform of type int.
  */
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

/**
  * \class Uniform<float>
  * \brief Specialization for uniform of type float.
  */
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

/**
  * \class ProgramBuilder
  * \brief Builder class for Program objects
  */
class ProgramBuilder {
    public:
        /**
          * \brief Constructor
          */
        ProgramBuilder();

        /**
          * \brief Destructor
          */
        ~ProgramBuilder();

        /**
          * \enum UniformType
          * \brief Possible types for uniform values
          */
        enum UniformType { vec3, mat3, mat4, int_, float_ };

        /**
          * \brief Attach a shader to the program.
          */
        ProgramBuilder& attach_shader(Shader&);

        /**
          * \brief Register a uniform
          */
        ProgramBuilder& with_uniform(std::string const& name, UniformType t);

        /**
          * \brief Link the program and return the Program object
          */
        Program link();

    private:
        GLuint m_program;
        std::vector<Shader*> m_attachedShaders;
        std::vector<std::pair<std::string, UniformType>> m_uniforms;
};

#undef TYPE

#endif
