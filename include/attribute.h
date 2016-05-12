#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include <string>
#include <vector>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/vec1.hpp>
#include <boost/mpl/size_t.hpp>

#include "vbo.h"

namespace Engine {

class MeshBuilder;

class AttributeArray {
    public:
        enum class Type { Int, Float, Double, Uchar, Ushort, Uint };
        enum class Kind { Positions, Normals, Colors, UVs, Indices };

        class Layout {
            public:
                Layout();
                Layout(int nComponents, Type type, size_t stride=0, bool normalize=false, std::intptr_t offset=0);
                Layout(Layout const& other);
                Layout& operator=(Layout const& other);

                int nComponents() const;
                Type type() const;
                size_t stride() const;
                bool normalize() const;
                std::intptr_t offset() const;

            private:
                int m_nComponents;
                Type m_type;
                size_t m_stride;
                bool m_normalize;
                std::intptr_t m_offset;
        };

        AttributeArray();
        AttributeArray(VBO const& vbo, Kind k, Layout const& l);
        AttributeArray(AttributeArray&& other);

        virtual ~AttributeArray();

        AttributeArray& operator=(AttributeArray&& other);

        AttributeArray* clone() const;

    protected:

        const VBO* m_vbo;
        Kind m_kind;
        Layout m_layout;
};

struct AttributeMap {
    AttributeArray positions;
    std::unique_ptr<AttributeArray> normals;
    std::unique_ptr<AttributeArray> colors;
    std::unique_ptr<AttributeArray> indices;
    std::unique_ptr<AttributeArray> uvs;

    AttributeMap();
    AttributeMap(AttributeMap&& other);
    AttributeMap& operator=(AttributeMap&& other);
};

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
} // namespace traits

} // namespace Engine

#endif
