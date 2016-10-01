#include <catch.hpp>

#include "utility.h"

using namespace Engine;

using T1 = type_list<char, int, float, double>;

TEST_CASE("Testing type_list_length metafunction", "[type_list]") {
    REQUIRE(type_list_length<T1>::value == 4);
    REQUIRE(type_list_length<type_list<>>::value == 0);
}

TEST_CASE("Testing type_list_get metafunction", "[type_list]") {
    bool b = std::is_same<typename type_list_get<T1, 0>::type, char>::value;
    REQUIRE(b);

    b = std::is_same<typename type_list_get<T1, 1>::type, int>::value;
    REQUIRE(b);

    b = std::is_same<typename type_list_get<T1, 2>::type, float>::value;
    REQUIRE(b);

    b = std::is_same<typename type_list_get<T1, 3>::type, double>::value;
    REQUIRE(b);
}
