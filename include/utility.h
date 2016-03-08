#ifndef UTILITY_H
#define UTILITY_H

#include <vector>
#include <fstream>
#include <cassert>

#define UNREACHABLE(t) ( (std::cerr << "Reached unreachable code at __FILE__:__LINE__" << std::endl, std::terminate(), t) )

float deg_to_rad(float angle);

float rad_to_deg(float angle);

template <class T>
void dump_binary(std::vector<T> const& vec, std::string const& filename);

#include "utility.inl"

#endif
