#ifndef TRANSFORM_H
#include "transform.h"
#endif

template <class O>
TransformBase<O>::TransformBase() :
    m_position(),
    m_orientation()
{ }

template <class O>
TransformBase<O>::TransformBase(glm::vec3 position, O orientation) :
    m_position(position),
    m_orientation(orientation)
{ }

template <class O>
TransformBase<O>::TransformBase(TransformBase&& other) :
    m_position(other.m_position),
    m_orientation(other.m_orientation)
{ }

template <class O>
TransformBase<O>::TransformBase(TransformBase const& other):
    m_position(other.m_position),
    m_orientation(other.m_orientation)
{ }

template <class O>
TransformBase<O>::~TransformBase() { }

template <class O>
TransformBase<O>& TransformBase<O>::operator=(TransformBase const& other) {
    m_position = other.m_position;
    m_orientation = other.m_orientation;
    return *this;
}

template <class O>
TransformBase<O>& TransformBase<O>::operator=(TransformBase&& other) {
    m_position = other.m_position;
    m_orientation = other.m_orientation;
    return *this;
}

template <class O>
glm::vec3 TransformBase<O>::position() const {
    return m_position;
}

template <class O>
O TransformBase<O>::orientation() const {
    return m_orientation;
}

template <class O>
void TransformBase<O>::set_position(glm::vec3 const& v) {
    m_position = v;
}

template <class O>
void TransformBase<O>::set_orientation(O const& o) {
    m_orientation = o;
}

template <class O>
glm::vec3 TransformBase<O>::x() const {
    return matrix()[0].xyz();
}

template <class O>
glm::vec3 TransformBase<O>::y() const {
    return matrix()[1].xyz();
}

template <class O>
glm::vec3 TransformBase<O>::z() const {
    return matrix()[2].xyz();
}

template <class O>
void TransformBase<O>::translate(glm::vec3 const& v) {
    m_position += v;
}

template <class O>
void TransformBase<O>::translate_local(Direction dir, float distance) {
    switch(dir) {
        case Direction::Front:
            translate(z() * -distance);
            break;
        case Direction::Back:
            translate(z() *  distance);
            break;
        case Direction::Right:
            translate(x() * distance);
            break;
        case Direction::Left:
            translate(x() * -distance);
            break;
        case Direction::Up:
            translate(y() * distance);
            break;
        case Direction::Down:
            translate(y() * -distance);
            break;
    }
}

template <class O>
void TransformBase<O>::translate_local(glm::vec3 const& v) {
    translate(v.x * x() +
              v.y * y() +
              v.z * z());
}

template <class O>
void TransformBase<O>::rotate_local(Axis axis, float angle) {
    switch(axis) {
        case Axis::X:
        rotate(glm::vec3(1, 0, 0), angle);
        break;
        case Axis::Y:
        rotate(glm::vec3(0, 1, 0), angle);
        break;
        case Axis::Z:
        rotate(glm::vec3(0, 0, 1), angle);
        break;
    }
}
