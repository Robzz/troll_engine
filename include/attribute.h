/**
  * \file include/attribute.h
  * \brief Contains the AttributeArray and AttributeMap definitions.
  * \author R.Chavignat
  */

#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include <string>
#include <vector>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/vec1.hpp>
#include <boost/mpl/size_t.hpp>

#include "vbo.h"
#include "utility.h"

namespace Engine {

class MeshBuilder;

/**
 * \class AttributeArray
 * \brief Represents an array of vertex attributes
 */
class AttributeArray {
    public:
        /**
         * \enum Type
         * \brief Represents the underlying data type of the attribute array.
         */
        enum class Type {
            /** Integer */
            Int,
            /** Floating point */
            Float,
            /** Double precision floating point */
            Double,
            /** Unsigned byte */
            Uchar,
            /** Unsigned 16bit integer */
            Ushort,
            /** Unsigned 32bit integer */
            Uint
        };
        /**
         * \enum Kind
         * \brief The 'kind' of data that the attribute array represents.
         */
        enum class Kind {
            /** Space coordinates */
            Positions,
            /** Normal vectors */
            Normals,
            /** Color data */
            Colors,
            /** Texture coordinates */
            UVs,
            /** Vertex indices */
            Indices
        };

        /**
         * @brief The Layout class describes the memory layout of an
         * attribute array.
         */
        class Layout {
            public:
                /**
                 * @brief Construct a new Layout object.
                 */
                Layout();
                /**
                 * @brief Construct a new initialized Layout object.
                 *
                 * @param nComponents Number of components
                 * @param type Type of the attribute
                 * @param stride Byte offset between consecutive attribute
                 * values. If the attribute is tightly packed, set stride to 0.
                 * @param normalize If true, the attribute values will be
                 * normalized when accessed from the GPU.
                 * @param offset Byte offset from the start of the buffer to
                 * the first attribute value.
                 */
                Layout(int nComponents, Type type, size_t stride=0, bool normalize=false, std::intptr_t offset=0);
                /**
                 * @brief Copy constructor
                 *
                 * @param other Layout object to copy
                 */
                Layout(Layout const& other);
                /**
                 * @brief Copy assignment operator
                 *
                 * @param other Layout objet to copy
                 *
                 * @return Reference to the assigned object
                 */
                Layout& operator=(Layout const& other);

                /**
                 * @brief Returns the number of components per attribute value
                 *
                 * @return The number of components per attribute value
                 */
                int nComponents() const;
                /**
                 * @brief Returns the type of the attribute array
                 *
                 * @return The type of the attribute array
                 */
                Type type() const;
                /**
                 * @brief Returns the stride of the attribute array
                 *
                 * @return The stride of the attribute array
                 */
                size_t stride() const;
                /**
                 * @brief Returns true if the attribute values are normalized
                 * when accessed from the GPU
                 *
                 * @return true if the attribute values are normalized when
                 * accessed from the GPU
                 */
                bool normalize() const;
                /**
                 * @brief Returns The byte offset from the start of the buffer
                 * to the first attribute value
                 *
                 * @return The byte offset from the start of the buffer to the
                 * first attribute value
                 */
                std::intptr_t offset() const;

            private:
                int m_nComponents;
                Type m_type;
                size_t m_stride;
                bool m_normalize;
                std::intptr_t m_offset;
        };

        /**
         * @brief Construct an empty AttributeArray.
         */
        AttributeArray();
        /**
         * @brief Construct and initialize an AttributeArray.
         *
         * @param vbo VBO containing the attribute array
         * @param k Kind of the attribute array
         * @param l Layout of the attribute array
         */
        AttributeArray(VBO const& vbo, Kind k, Layout const& l);
        /**
         * @brief Move constructor
         *
         * @param other AttributeArray instance to move
         */
        AttributeArray(AttributeArray&& other);
        /**
         * @brief Move assignment operator
         *
         * @param other AttributeArray instance to move
         *
         * @return Reference to the assigned AttributeArray
         */
        AttributeArray& operator=(AttributeArray&& other);

        /* No copy */
        AttributeArray(AttributeArray const& other) = delete;
        AttributeArray& operator=(AttributeArray const& other) = delete;

        /**
         * @brief Destructor.
         */
        virtual ~AttributeArray();

        // TODO: delete this?
        AttributeArray* clone() const;

        const VBO* vbo;
        Kind kind;
        Layout layout;
};

/**
 * \class AttributeMap
 * \brief The attribute map of a loaded Mesh.
 */
struct AttributeMap {
    /** Vertex positions */
    AttributeArray positions;
    /** Vertex normals, optional */
    std::unique_ptr<AttributeArray> normals;
    /** Vertex colors, optional */
    std::unique_ptr<AttributeArray> colors;
    /** Vertex indices, optional */
    std::unique_ptr<AttributeArray> indices;
    /** Vertex texture coordinates, optional */
    std::unique_ptr<AttributeArray> uvs;

    /**
     * @brief Construct an empty AttributeMap
     */
    AttributeMap();
    /**
     * @brief Move constructor.
     *
     * @param other AttributeMap object to move
     */
    AttributeMap(AttributeMap&& other);
    /**
     * @brief Move assignment operator
     *
     * @param other AttributeMap object to move
     *
     * @return Reference to the assigned AttributeMap
     */
    AttributeMap& operator=(AttributeMap&& other);
};

#ifndef DOX_SKIP_BLOCK
namespace traits {
    template <typename T>
    struct dimension : boost::mpl::size_t<1> { };

    template <>
    struct dimension<glm::vec1> : boost::mpl::size_t<1> { } ;
    template <>
    struct dimension<glm::vec2> : boost::mpl::size_t<2> { } ;
    template <>
    struct dimension<glm::vec3> : boost::mpl::size_t<3> { } ;
    template <>
    struct dimension<glm::vec4> : boost::mpl::size_t<4> { } ;

    template <typename T>
    struct attribute_type_enumerator { };
    template <>
    struct attribute_type_enumerator<glm::vec1> {
        static constexpr AttributeArray::Type value = AttributeArray::Type::Float;
    };
    template <>
    struct attribute_type_enumerator<glm::vec2> {
        static constexpr AttributeArray::Type value = AttributeArray::Type::Float;
    };
    template <>
    struct attribute_type_enumerator<glm::vec3> {
        static constexpr AttributeArray::Type value = AttributeArray::Type::Float;
    };
    template <>
    struct attribute_type_enumerator<glm::vec4> {
        static constexpr AttributeArray::Type value = AttributeArray::Type::Float;
    };
    template <>
    struct attribute_type_enumerator<unsigned char> {
        static constexpr AttributeArray::Type value = AttributeArray::Type::Uchar;
    };
    template <>
    struct attribute_type_enumerator<unsigned short> {
        static constexpr AttributeArray::Type value = AttributeArray::Type::Ushort;
    };
    template <>
    struct attribute_type_enumerator<unsigned int> {
        static constexpr AttributeArray::Type value = AttributeArray::Type::Uint;
    };

    template <size_t dim>
    struct vec_int { };
    template <>
    struct vec_int<1> { typedef glm::ivec1 type; };
    template <>
    struct vec_int<2> { typedef glm::ivec2 type; };
    template <>
    struct vec_int<3> { typedef glm::ivec3 type; };
    template <>
    struct vec_int<4> { typedef glm::ivec4 type; };

    template <size_t dim>
    struct vec_float { };
    template <>
    struct vec_float<1> { typedef glm::vec1 type; };
    template <>
    struct vec_float<2> { typedef glm::vec2 type; };
    template <>
    struct vec_float<3> { typedef glm::vec3 type; };
    template <>
    struct vec_float<4> { typedef glm::vec4 type; };

    template <size_t dim>
    struct vec_double { };
    template <>
    struct vec_double<1> { typedef glm::dvec1 type; };
    template <>
    struct vec_double<2> { typedef glm::dvec2 type; };
    template <>
    struct vec_double<3> { typedef glm::dvec3 type; };
    template <>
    struct vec_double<4> { typedef glm::dvec4 type; };

    template <typename T, size_t d>
    struct concrete_attribute_type { };
    template <size_t dim>
    struct concrete_attribute_type<float, dim> { typedef typename vec_float<dim>::type type; };
    template <size_t dim>
    struct concrete_attribute_type<int, dim> { typedef typename vec_int<dim>::type type; };
    template <size_t dim>
    struct concrete_attribute_type<double, dim> { typedef typename vec_double<dim>::type type; };

    template <typename T>
    struct vertex_index_type {
        typedef typename std::enable_if<std::is_same<T, unsigned char>::value ||
                                        std::is_same<T, unsigned short>::value ||
                                        std::is_same<T, unsigned int>::value, T> type;
    };

    template <>
    struct gl_value<AttributeArray::Type> {
        static GLenum value(AttributeArray::Type t) {
            switch(t) {
                case AttributeArray::Type::Float:
                    return GL_FLOAT;
                case AttributeArray::Type::Double:
                    return GL_DOUBLE;
                case AttributeArray::Type::Uint:
                    return GL_UNSIGNED_INT;
                case AttributeArray::Type::Uchar:
                    return GL_UNSIGNED_BYTE;
                case AttributeArray::Type::Ushort:
                    return GL_UNSIGNED_SHORT;
                case AttributeArray::Type::Int:
                    return GL_INT;
            }
            UNREACHABLE(0);
        }
    };
} // namespace traits
#endif // DOX_SKIP_BLOCK

} // namespace Engine

#endif
