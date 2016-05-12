#include "object/object.h"

namespace Engine {

Object::Object() { }

Object::Object(Object const& other) { }

Object::Object(Object&& other) { }

Object::~Object() { }

Object& Object::operator=(Object const& other) {
    return *this;
}

Object& Object::operator=(Object&& other) {
    return *this;
}

} // namespace Engine
