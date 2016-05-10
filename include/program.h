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
        ProgramHandle(GLuint handle);
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
        ShaderManager(bool cache = true);
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
        void use();
        // Disable programs for rendering
        static void noProgram();

        // Return the OpenGL location of an attribute
        GLint getAttributeLocation(std::string) const;
        // Return the Uniform object associated with an uniform
        UniformBase* getUniform(std::string const& name);
        // Upload uniforms to GPU
        void uploadUniforms();

        bool is_current() const;
        static Program* current();
    
        ~Program();

    private:
        Program(std::shared_ptr<ProgramHandle> h, std::vector<UniformBase*> uniforms);
        
        // Not copyable
        Program(Program const& other);
        Program& operator=(Program const& other);

        GLint getUniformLocation(std::string) const;

        std::shared_ptr<ProgramHandle> m_id;
        std::vector<UniformBase*> m_uniforms;
        static Program* s_current;
};

template <class T>
void upload_uniform(const GLint location, T const& value);

// Class for Uniform objects
template <class T>
class Uniform : public UniformBase {
    friend class ProgramBuilder;
    public:
        /* Destructor */
        virtual ~Uniform();

        /* Modify the uniform value. Does not upload to GPU. */
        void set(T const& value);

    private:
        T m_value;

        /* Default constructor */
        Uniform(GLint location, std::string const& name);

        /* Upload uniform to GPU. Program must be in use. */
        virtual void upload();
};

// Use this class to build Program objects
class ProgramBuilder {
    public:
    friend class ShaderManager;
        ~ProgramBuilder();

        enum class UniformType { Vec3, Mat3, Mat4, Int, Float };

        /* Attach a shader to the program */
        ProgramBuilder& vertexShader(std::string const& file);
        ProgramBuilder& fragmentShader(std::string const& file);
        ProgramBuilder& geometryShader(std::string const& file);

        /* Register a uniform */
        ProgramBuilder& uniform(std::string const& name, UniformType t);

        /* Link the program and return the Program object */
        Program build();

    private:
        ProgramBuilder(ShaderManager& manager);
        ShaderManager& m_manager;
        std::vector<Shader*> m_shaders;
        std::vector<std::pair<std::string, UniformType>> m_uniforms;

        template<Shader::Type T>
        Shader* compileShader(std::string const&);
};

#include "program.inl"

} // namespace Engine

#undef TYPE

#endif // PROGRAM_H
