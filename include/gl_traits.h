/**
  * \file include/gl_traits.h
  * \brief Contains various traits related to OpenGL types used for
  * metaprogramming.
  * \author R.Chavignat
  */

#ifndef GL_TRAITS_H
#define GL_TRAITS_H

namespace Engine {
    namespace traits {
        /**
         * @brief Meta-function returning the GLSL alignment of a specified type.
         *
         * @tparam T GLSL-compatible type.
         */
        template <class T>
        struct glsl_alignment;

        template <class T>
        struct glsl_alignment<T&> {
            static constexpr size_t value = glsl_alignment<typename std::remove_cv<typename std::remove_reference<T>::type>::type>::value;
        };

        template <>
        struct glsl_alignment<float> {
            static constexpr size_t value = 4;
        };

        template <>
        struct glsl_alignment<glm::vec3> {
            static constexpr size_t value = 16;
        };

    } // namespace traits
} // namespace Engine

#endif // GL_TRAITS_H
