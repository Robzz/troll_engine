#ifndef UTILITY_H
#define UTILITY_H

#include <vector>
#include <fstream>
#include <cassert>
#include <type_traits>
#include "gl_core_3_3.h"

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

    template <class T>
    struct gl_value { };
}

template <class E>
typename std::enable_if<traits::enable_bitmask_operators<E>::enable, E>::type operator|(E e1, E e2);
template <class E>
typename std::enable_if<traits::enable_bitmask_operators<E>::enable, E>::type operator&(E e1, E e2);

#include "utility.inl"

} // namespace Engine

#endif
