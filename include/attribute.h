#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/vec1.hpp>
#include <boost/mpl/size_t.hpp>

namespace Engine {

template <typename T>
class AttributeArrayInstance;

class AttributeArray {
    public:
        enum class Type { Int, Float, Double };
        /* Default constructor */
        AttributeArray(Type t);
        /* Destructor */
        virtual ~AttributeArray();

        template <typename T>
        const AttributeArrayInstance<T>* downcast() const;
        template <typename T>
        AttributeArrayInstance<T>* downcast();

        virtual size_t size() const = 0;
        virtual size_t dimension() const = 0;
        virtual AttributeArray* clone() const = 0;

    protected:
        const Type m_type;
};

template <class T>
class AttributeArrayInstance : public AttributeArray {
    public:
        /* Default constructor */
        AttributeArrayInstance();
        /* Destructor */
        virtual ~AttributeArrayInstance();

        /* Copy constructor */
        AttributeArrayInstance(AttributeArrayInstance const& other);
        /* Move constructor */
        AttributeArrayInstance(AttributeArrayInstance&& other);

        /* Assignment operator */
        AttributeArrayInstance& operator=(AttributeArrayInstance const& other);
        /* Move-assignment operator */
        AttributeArrayInstance& operator=(AttributeArrayInstance&& other);

        virtual size_t size() const;

        virtual AttributeArray* clone() const;
        constexpr size_t dimension() const;

        std::vector<T>& data();
        std::vector<T> const& data() const;

    private:
        std::vector<T> m_attribData;
};

namespace traits {
    template <typename T>
    struct dimension { };

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

template <class T>
class ElementArray : public AttributeArrayInstance<typename traits::vertex_index_type<T>::type>{
    public:
        /* Default constructor */
        ElementArray();
        /* Destructor */
        virtual ~ElementArray();
};


#include "attribute.inl"

} // namespace Engine

#endif
