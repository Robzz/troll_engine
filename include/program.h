#ifndef PROGRAM_H
#define PROGRAM_H

#include "shader.h"
#include "gl_core_3_3.h"
#include "debug.h"

#include <map>
#include <set>
#include <memory>
#include <typeinfo>
#include <sstream>
#include <vector>
#include <type_traits>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#define TYPE(T) typeid(T).hash_code()

namespace Engine {

/**
  * \class ProgramHandle
  * \brief Owns the OpenGL handle to a Program.
  */
class ProgramHandle {
    public:
        /* Default constructor */
        explicit ProgramHandle(GLuint handle);
        /* Destructor */
        virtual ~ProgramHandle();

        ProgramHandle(ProgramHandle const& other) = delete;
        ProgramHandle(ProgramHandle&& other) = delete;
        ProgramHandle& operator=(ProgramHandle const& other) = delete;
        ProgramHandle& operator=(ProgramHandle&& other) = delete;

        GLuint value() const;

    private:
        GLuint m_handle;
};

class Program;
class ProgramBuilder;

/**
  * \class ShaderManager
  * \brief This class manages shaders and programs. It can cache previously compiled shaders
  * and previously linked programs.
  */
class ShaderManager {
    friend class ProgramBuilder;
    public:
        /* Default constructor */
        explicit ShaderManager(bool cache = true);
        /* Destructor */
        virtual ~ShaderManager();

        ShaderManager(ShaderManager const& other) = delete;
        ShaderManager(ShaderManager&& other) = delete;
        ShaderManager& operator=(ShaderManager const& other) = delete;
        ShaderManager& operator=(ShaderManager&& other) = delete;

        ProgramBuilder buildProgram();

    private:
        bool m_cache;
        std::map<std::string, Shader*> m_shaderCache;
        std::map<std::set<Shader*>, std::shared_ptr<ProgramHandle>> m_programCache;

        void compileShader(std::string shaderFile);
};

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
        Program();

        /**
          * \brief Move constructor.
          */
        Program(Program&& other);
        Program& operator=(Program&& other);

        // No copy
        Program(Program const& other) = delete;
        Program& operator=(Program const& other) = delete;

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
        Program(std::shared_ptr<ProgramHandle> h, std::vector<UniformBase*> uniforms);

        /**
          * \brief Return the location of a uniform
          * \param uni Name of the uniform whose location to query.
          */
        GLint getUniformLocation(std::string const& uni) const;

        std::shared_ptr<ProgramHandle> m_id;
        std::vector<UniformBase*> m_uniforms;
        static const Program* s_current;
};

template <class T>
void upload_uniform(const GLint location, T const& value);

// Class for Uniform objects
template <class T>
class Uniform : public UniformBase {
    friend class ProgramBuilder;
    public:
        /**
          * \brief Destructor
          */
        virtual ~Uniform();

        /**
          * \brief Set the uniform value, do not upload to the GPU.
          * \param value New value.
          */
        void set(T const& value);

    private:
        T m_value;

        /**
          * \brief Default constructor
          * \param location OpenGL uniform location
          * \param name Uniform name
          */
        Uniform(GLint location, std::string const& name);

        /**
          * \brief Upload uniform to GPU. Program must be in use.
          */
        virtual void upload();
};

/**
  * \class ProgramBuilder
  * \brief Builder class for Program objects
  */
class ProgramBuilder {
    public:
    friend class ShaderManager;
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
        enum UniformType { Vec3, Mat3, Mat4, Int, Float };

        /* Attach a shader to the program */
        ProgramBuilder& vertexShader(std::string const& file);
        ProgramBuilder& fragmentShader(std::string const& file);
        ProgramBuilder& geometryShader(std::string const& file);

        /**
          * \brief Register a uniform
          */
        ProgramBuilder& uniform(std::string const& name, UniformType t);

        /**
          * \brief Link the program and return the Program object
          */
        Program build();

    private:
        explicit ProgramBuilder(ShaderManager& manager);
        ShaderManager* m_manager;
        std::vector<Shader*> m_shaders;
        std::vector<std::pair<std::string, UniformType>> m_uniforms;

        Shader* compileShader(std::string const&, Shader::Type t);
};

#include "program.inl"

} // namespace Engine

#undef TYPE

#endif
