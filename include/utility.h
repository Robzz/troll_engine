#ifndef UTILITY_H
#define UTILITY_H

#include <vector>
#include <fstream>
#include <cassert>
#include <type_traits>

#include <glm/glm.hpp>
#include <glm/gtc/vec1.hpp>
#include <boost/mpl/size_t.hpp>

#include <glbinding/gl33core/gl.h>

#define UNREACHABLE(t) ( (std::cerr << "Reached unreachable code at __FILE__:__LINE__" << std::endl, std::terminate(), t) )

namespace Engine {

float deg_to_rad(float angle);

float rad_to_deg(float angle);

template <class T>
void dump_binary(std::vector<T> const& vec, std::string const& filename);

namespace traits {
    template <class E>
    struct enable_bitmask_operators {
        static constexpr bool enable = false;
    };

    template <class E>
    struct enum_class_value {
        static constexpr bool enable = false;
    };

    template <class T>
    struct gl_value { };

    /**
     * \brief Specifies the dimension of vector types
     * \tparam T Vector type
     */
    template <typename T>
    struct dimension;

    /** @brief Specialization of \ref dimension for glm::vec1 */
    template <>
    struct dimension<glm::vec1> : boost::mpl::size_t<1> { } ;

    /** @brief Specialization of \ref dimension for glm::vec2 */
    template <>
    struct dimension<glm::vec2> : boost::mpl::size_t<2> { } ;

    /** @brief Specialization of \ref dimension for glm::vec3 */
    template <>
    struct dimension<glm::vec3> : boost::mpl::size_t<3> { } ;

    /** @brief Specialization of \ref dimension for glm::vec4 */
    template <>
    struct dimension<glm::vec4> : boost::mpl::size_t<4> { } ;
}

template <class E>
typename std::enable_if<traits::enable_bitmask_operators<E>::enable, E>::type operator|(E e1, E e2);
template <class E>
typename std::enable_if<traits::enable_bitmask_operators<E>::enable, E>::type operator&(E e1, E e2);

template <class E>
typename std::enable_if<traits::enum_class_value<E>::enable, typename std::underlying_type<E>::type>::type value(E e);

/**
 * @brief Type representing a list of types.
 *
 * @tparam T List of types
 */
template <class... T>
struct type_list { };

/**
 * @brief Meta-function returning the number of types in a type list.
 *
 * @tparam T type_list type
 */
template <class T>
struct type_list_length;

#ifndef DOX_SKIP_BLOCK
template <class H, class... T>
struct type_list_length<type_list<H, T...>> : boost::mpl::size_t<1 + type_list_length<type_list<T...>>::value> { };

template <>
struct type_list_length<type_list<>> : boost::mpl::size_t<0> { };
#endif // DOX_SKIP_BLOCK

template <class T, unsigned int i>
struct type_list_get;

#ifndef DOX_SKIP_BLOCK
template <class H, class... T, unsigned int i>
struct type_list_get<type_list<H, T...>, i> {
    static_assert(i < type_list_length<type_list<H, T...>>::value, "type_list access out of bounds");
    using type = typename type_list_get<type_list<T...>, i-1>::type;
};

template <class H, class... T>
struct type_list_get<type_list<H, T...>, 0> {
    using type = H;
};

template <intptr_t addr, ptrdiff_t alignment>
struct align : std::integral_constant<intptr_t,
    (addr % alignment == 0) ?
    addr :
    (addr + alignment - addr % alignment)> { };

template <unsigned int i>
struct type_list_get<type_list<>, i>;

#endif // DOX_SKIP_BLOCK

/**
 * @brief Meta-function returning a no-op function taking the specified
 * parameter list.
 *
 * @tparam T Type list
 */
template <class... T>
struct no_op {
    static void f(T...) {};
};

#include "utility.inl"

} // namespace Engine

#endif
