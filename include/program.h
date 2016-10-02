/**
  * \file include/program.h
  * \brief Contains the definitions related to shader programs.
  * \author R.Chavignat
  */
#ifndef PROGRAM_H
#define PROGRAM_H

#include "shader.h"
#include <glbinding/gl33core/gl.h>

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
        /**
         * \brief Constructor
         *
         * \param handle OpenGL shader program handle
         */
        explicit ProgramHandle(gl::GLuint handle);

        /**
         * \brief Destructor
         */
        virtual ~ProgramHandle();

        ProgramHandle(ProgramHandle const& other) = delete;
        ProgramHandle(ProgramHandle&& other) = delete;
        ProgramHandle& operator=(ProgramHandle const& other) = delete;
        ProgramHandle& operator=(ProgramHandle&& other) = delete;

        /**
         * \brief Returns the OpenGL shader program handle.
         *
         * \return The OpenGL shader program handle
         */
        gl::GLuint value() const;

    private:
        gl::GLuint m_handle;
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
        /**
         * \brief ShaderManager constructor.
         *
         * \param cache If set to true, the ShaderManager will cache the
         * managed shaders to avoid recompilations.
         */
        explicit ShaderManager(bool cache = true);

        /**
         * \brief Destructor.
         */
        virtual ~ShaderManager();

        ShaderManager(ShaderManager const& other) = delete;
        ShaderManager(ShaderManager&& other) = delete;
        ShaderManager& operator=(ShaderManager const& other) = delete;
        ShaderManager& operator=(ShaderManager&& other) = delete;

        /**
         * \brief Returns a ProgramBuilder instance to build a new Program.
         *
         * \return A ProgramBuilder instance.
         */
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
        UniformBase(gl::GLint location, std::string const& name);

        /** \brief OpenGL uniform location */
        gl::GLint m_location;

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
        gl::GLint getAttributeLocation(std::string const& name) const;

        gl::GLuint getUniformBlockIndex(std::string const&);

        void uniformBlockBinding(unsigned int index, unsigned int binding);

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
        Program(std::shared_ptr<ProgramHandle> id, std::vector<UniformBase*> uniforms);

        /**
          * \brief Return the location of a uniform
          * \param uni Name of the uniform whose location to query.
          */
        gl::GLint getUniformLocation(std::string const& uni) const;

        std::shared_ptr<ProgramHandle> m_id;
        std::vector<UniformBase*> m_uniforms;
        static const Program* s_current;
};

/**
 * \brief Upload a uniform to the GPU.
 *
 * \tparam T Uniform type
 * \param location Uniform location
 * \param value Uniform value
 */
template <class T>
void upload_uniform(const gl::GLint location, T const& value);

/**
 * \class Uniform
 * \brief Uniform class.
 * \tparam T Uniform type
 */
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
        Uniform(gl::GLint location, std::string const& name);

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

        /**
         * @brief Attach a vertex shader to the program
         *
         * @param file Path to a GLSL shader code file
         *
         * @return Reference to the ProgramBuilder
         */
        ProgramBuilder& vertexShader(std::string const& file);

        /**
         * @brief Attach a fragment shader to the program
         *
         * @param file Path to a GLSL shader code file
         *
         * @return Reference to the ProgramBuilder
         */
        ProgramBuilder& fragmentShader(std::string const& file);

        /**
         * @brief Attach a geometry shader to the program
         *
         * @param file Path to a GLSL shader code file
         *
         * @return Reference to the ProgramBuilder
         */
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

        Shader* compileShader(std::string const&, gl::GLenum t);
};

#include "program.inl"

} // namespace Engine

#undef TYPE

#endif
