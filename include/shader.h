/**
  * \file shader.h
  * \brief %Shader class definitions
  * \author R.Chavignat
  */
#ifndef SHADER_H
#define SHADER_H

#include <istream>
#include "gl_core_3_3.h"

/**
  * \class Shader
  * \brief Base class for shader classes.
  */
class Shader {
    friend class ProgramBuilder;
    public:
        /**
          * \brief Destructor
          */
        virtual ~Shader();
        /**
          * \brief Check if the shader handle is valid.
          */
        operator bool() const;
        /**
          * \brief Check if the shader handle is invalid.
          */
        bool operator !() const;
        /**
          * \brief Return the shader information log.
          */
        std::string info_log() const;

    protected:
        Shader(std::istream& src_stream, GLenum shader_type);
        GLuint m_id;
};

/**
  * \class VertexShader
  * \brief Represents a vertex shader.
  */
class VertexShader : public Shader {
    public:
         explicit VertexShader(std::istream& src_stream);
         virtual ~VertexShader();
};

/**
  * \class FragmentShader
  * \brief Represents a fragment shader.
  */
class FragmentShader : public Shader {
    public:
         explicit FragmentShader(std::istream& src_stream);
         virtual ~FragmentShader();
};

/**
  * \class GeometryShader
  * \brief Represents a geometry shader.
  */
class GeometryShader : public Shader {
    public:
         explicit GeometryShader(std::istream& src_stream);
         virtual ~GeometryShader();
};

#endif
