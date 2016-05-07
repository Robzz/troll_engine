#include "transform.h"
#include <iostream>
#include "debug.h"
#include "utility.h"

namespace Engine {

Direction operator-(Direction d) {
    switch(d) {
        case Direction::Up:
            return Direction::Down;
        case Direction::Down:
            return Direction::Up;
        case Direction::Left:
            return Direction::Right;
        case Direction::Right:
            return Direction::Left;
        case Direction::Front:
            return Direction::Back;
        case Direction::Back:
            return Direction::Front;
    }
    return UNREACHABLE(Direction::Down);
}

TransformMat::TransformMat() :
    TransformBase<glm::mat3>()
{ }

TransformMat::TransformMat(glm::vec3 position, glm::mat3 orientation) :
    TransformBase<glm::mat3>(position, orientation)
{ }

TransformMat::~TransformMat() { }

glm::vec3 TransformMat::x() const { return m_orientation[0].xyz(); }
glm::vec3 TransformMat::y() const { return m_orientation[1].xyz(); }
glm::vec3 TransformMat::z() const { return m_orientation[2].xyz(); }

void TransformMat::rotate(glm::vec3 const& axis, float angle) {
    m_orientation *= glm::mat3(glm::rotate(glm::mat4(1), angle, axis));
    m_orientation = glm::orthonormalize(m_orientation);
}

glm::mat4 TransformMat::matrix() const {
    return glm::translate(glm::mat4(1), m_position) * glm::mat4(m_orientation);
}

TransformEuler::TransformEuler() :
    TransformBase<glm::vec3>()
    { }

TransformEuler::TransformEuler(glm::vec3 position, glm::vec3 orientation) :
    TransformBase<glm::vec3>(position, orientation)
    { }

TransformEuler::~TransformEuler() { }

glm::vec3 TransformEuler::x() const {
    TransformMat t = euler_to_matrix(*this);
    return t.x();
}

glm::vec3 TransformEuler::y() const {
    return euler_to_matrix(*this).y();
}

glm::vec3 TransformEuler::z() const {
    return euler_to_matrix(*this).z();
}

void TransformEuler::rotate(glm::vec3 const& axis, float angle) {
    TransformMat t = euler_to_matrix(*this);
    t.rotate(axis, angle);
    *this = matrix_to_euler(t);
}

void TransformEuler::rotate_local(Axis axis, float angle) {
    switch(axis) {
        case Axis::X:
            m_orientation.y += angle;
            break;
        case Axis::Y:
            m_orientation.x += angle;
            break;
        case Axis::Z:
            m_orientation.z += angle;
            break;
    }
}

glm::mat4 TransformEuler::matrix() const {
    glm::mat4 m(1);
    m = glm::translate(glm::mat4(m), m_position);
    m = glm::rotate(m, m_orientation.x, glm::vec3(0, 1, 0));
    m = glm::rotate(m, m_orientation.y, glm::vec3(1, 0, 0));
    m = glm::rotate(m, m_orientation.z, glm::vec3(0, 0, 1));
    return m;
}

TransformMat euler_to_matrix(TransformEuler const& t) {
    return TransformMat(t.position(), glm::mat3(t.matrix()));
}

TransformEuler matrix_to_euler(TransformMat const& t) {
    glm::mat3 const& m = glm::transpose(t.orientation());
    float yaw, pitch, roll, m21 = m[2][1];
    pitch = (m21 <= -1.f) ? -M_PI/2 :
            (m21 >= 1.f)  ?  M_PI/2 :
                             asin(m21);
    if(fabs(m21) > 0.9999) {
        // Gimbal lock case
        roll = 0;
        yaw = atan2(-m[0][2], m[0][0]);
    }
    else {
        roll = atan2(m[2][0], m[2][2]);
        yaw  = atan2(m[0][1], m[1][1]);
    }
    return TransformEuler(t.position(), glm::vec3(yaw, pitch, roll));
}

} // namespace Engine
