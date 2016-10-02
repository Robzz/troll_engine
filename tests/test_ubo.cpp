#include <catch.hpp>
#include <glm/glm.hpp>

#include "ubo.h"
#include "utility.h"

struct T1 {
    glm::vec3 v1;
    glm::vec3 v2;
};

struct T2 {
    glm::vec3 v1;
    float f;
    glm::vec3 v2;
};

namespace Engine {
    namespace traits {
        template <>
        struct uniform_block_types<T1> {
            using type = type_list<glm::vec3, glm::vec3>;
        };

        template <>
        struct uniform_block_types<T2> {
            using type = type_list<glm::vec3, float, glm::vec3>;
        };
    } // namespace traits
} // namespace Engine

using namespace Engine;

TEST_CASE("Testing block_size", "[ubo-traits]") {
    REQUIRE(traits::block_size<T1>::value == 28);
    REQUIRE(traits::block_size<T2>::value == 28);
}

TEST_CASE("Testing n_fields", "[ubo-traits]") {
    REQUIRE(traits::n_fields<T1>::value == 2);
    REQUIRE(traits::n_fields<T2>::value == 3);
}

TEST_CASE("Testing field_type", "[ubo-traits]") {
    bool b = std::is_same<traits::field_type<T1, 0>, glm::vec3>::value;
    REQUIRE(b);
    b = std::is_same<traits::field_type<T1, 1>, glm::vec3>::value;
    REQUIRE(b);
    b = std::is_same<traits::field_type<T2, 0>, glm::vec3>::value;
    REQUIRE(b);
    b = std::is_same<traits::field_type<T2, 1>, float>::value;
    REQUIRE(b);
    b = std::is_same<traits::field_type<T2, 2>, glm::vec3>::value;
    REQUIRE(b);
}

TEST_CASE("Testing field_std140_offset", "[ubo-traits]") {
    bool b = (traits::field_std140_offset<T1, 0>::value == 0);
    REQUIRE(b);
    b = (traits::field_std140_offset<T1, 1>::value == 16);
    REQUIRE(b);
    b = (traits::field_std140_offset<T2, 0>::value == 0);
    REQUIRE(b);
    b = (traits::field_std140_offset<T2, 1>::value == 12);
    REQUIRE(b);
    b = (traits::field_std140_offset<T2, 2>::value == 16);
    REQUIRE(b);
}

TEST_CASE("Testing field_struct_offset", "[ubo-traits]") {
    bool b = (traits::field_struct_offset<T1, 0>::value == 0);
    REQUIRE(b);
    b = (traits::field_struct_offset<T1, 1>::value == 12);
    REQUIRE(b);
    b = (traits::field_struct_offset<T2, 0>::value == 0);
    REQUIRE(b);
    b = (traits::field_struct_offset<T2, 1>::value == 12);
    REQUIRE(b);
    b = (traits::field_struct_offset<T2, 2>::value == 16);
    REQUIRE(b);
}

TEST_CASE("Testing get_field", "[ubo-traits]") {
    T1 t1;
    t1.v1 = glm::vec3(1.f);
    t1.v2 = glm::vec3(2.f);
    bool b = traits::get_field<T1, 0>::value(t1) == t1.v1;
    REQUIRE(b);
    b = traits::get_field<T1, 1>::value(t1) == t1.v2;
    REQUIRE(b);

    T2 t2;
    t2.v1 = glm::vec3(1.f);
    t2.f = 5.f;
    t2.v2 = glm::vec3(2.f);
    b = traits::get_field<T2, 0>::value(t2) == t2.v1;
    REQUIRE(b);
    b = traits::get_field<T2, 1>::value(t2) == t2.f;
    REQUIRE(b);
    b = traits::get_field<T2, 2>::value(t2) == t2.v2;
    REQUIRE(b);
}
