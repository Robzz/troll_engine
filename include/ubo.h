/**
  * \file include/ubo.h
  * \brief Contains the definition of the UBO class
  * \author R.Chavignatt
  */
#ifndef UBO_H
#define UBO_H

#include "vbo.h"
#include "utility.h"
#include "gl_traits.h"

#include <vector>

namespace Engine {
    struct UniformBlock { };

    namespace traits {
        /**
         * @brief Trait used to specify the types present in a uniform block type
         *
         * @tparam U type_list object.
         */
        template <class U>
        struct uniform_block_types;

        /**
         * @brief Return the number of fields in a uniform block type.
         *
         * @tparam U Uniform block type
         */
        template <class U>
        struct n_fields : boost::mpl::size_t<type_list_length<typename uniform_block_types<U>::type>::value> { };

        template <class U, unsigned int i>
        using field_type = typename type_list_get<typename uniform_block_types<U>::type, i>::type;

        /**
         * @brief Return the byte offset of the i-th field of a uniform block
         * from the start of the block in GLSL std140 layout.
         *
         * @tparam U Uniform block type
         * @tparam i Field index
         */
        template <class U, unsigned int i>
        struct field_std140_offset {
            using T = field_type<U, i>;
            static constexpr ptrdiff_t value =
                align<(field_std140_offset<U, i-1>::value + sizeof(field_type<U, i-1>)), glsl_alignment<T>::value>::value;
        };

        /**
         * @brief Return the byte offset of the i-th field of a uniform block
         * from the start of the block in the C++ struct layout.
         *
         * @tparam U Uniform block type
         * @tparam i Field index
         */
        template <class U, unsigned int i>
        struct field_struct_offset {
            using T = field_type<U, i>;
            static constexpr ptrdiff_t value =
                align<field_struct_offset<U, i-1>::value + sizeof(field_type<U, i-1>), alignof(T)>::value;
        };

        /**
         * @brief Returns a reference to the i-th field of the uniform block.
         *
         * @tparam U Uniform block type
         * @tparam i Field index
         */
        template <class U, unsigned int i>
        struct get_field {
            static_assert(i < n_fields<U>::value, "Field index out of bounds");
            using T = field_type<U, i>;

            static T const& value(U const& data) {
                return reinterpret_cast<T const&>(*(reinterpret_cast<const char*>(&data) + field_struct_offset<U, i>::value));
            }
        };

#ifndef DOX_SKIP_BLOCK
        // Specialization of field_offset for i = 0 to terminate recursion.
        template <class U>
        struct field_std140_offset<U, 0> {
            static constexpr ptrdiff_t value = 0;
        };

        template <class U>
        struct field_struct_offset<U, 0> {
            static constexpr ptrdiff_t value = 0;
        };

        // Private implementation of UBO::upload_std140
        template <class U, unsigned int i>
        struct uploadUBOStd140Impl {
            static inline void f(U const& block, VBO& v);

            static inline void upload_field(U const& block, VBO& v, ptrdiff_t offset);
        };
#endif // DOX_SKIP_BLOCK

        /**
         * @brief Return the required size in bytes for a uniform block type
         * in the std_140 storage layout.
         *
         * @tparam U Uniform block type
         */
        template <class U>
        struct block_size : boost::mpl::size_t<field_std140_offset<U, n_fields<U>::value-1>::value + sizeof(field_type<U, n_fields<U>::value-1>)> { };


    } // namespace traits

    /**
     * @brief Represents an OpenGL Uniform Buffer Object.
     *
     * @tparam T Type of the uniform block
     */
    template <class T>
    class UBO: public VBO {
        //static_assert(std::is_base_of<UniformBlock, T>::value, "UBO type parameter must inherit from UniformBlock");

        public:
            /**
             * @brief Uniform block data.
             */
            T data;

            /**
             * @brief Construct an empty UBO.
             */
            UBO();


            /**
             * @brief Destroy the UBO.
             */
            virtual ~UBO();

            /**
             * @brief Upload the uniform block to the GPU.
             *
             * @param hint Buffer usage hint
             */
            void upload_std140(GLenum hint = GL_DYNAMIC_DRAW);

            /*
             * @brief Upload an array of uniform blocks to the GPU.
             *
             * @param data Uniform block array
             * @param hint Buffer usage hint
             */
            // TODO: implement this later as a class template specialization
            //void upload_std140(std::vector<T> const& data, GLenum hint = GL_DYNAMIC_DRAW);
    };

#include "ubo.inl"

} // namespace Engine

#endif
