#include "camera.h"
#include "glm/gtc/matrix_transform.hpp"

Camera::Camera() :
    m_up(0, 1, 0),
    m_right(1, 0, 0),
    m_front(0, 0, -1),
    m_position(0, 0, 0),
    m_orientation(1, 0, 0, 0)
{ }

Camera::Camera(Camera const& other) :
    m_up(other.m_up),
    m_right(other.m_right),
    m_front(other.m_front),
    m_position(other.m_position),
    m_orientation(other.m_orientation)
{ }

Camera& Camera::operator=(Camera const& other) {
    m_up = other.m_up;
    m_right = other.m_right;
    m_front = other.m_front;
    m_position = other.m_position;
    m_orientation = other.m_orientation;

    return *this;
}

Camera::~Camera() { }

void Camera::translate(Camera::Direction dir, float distance) {
    switch(dir) {
        case Front:
            m_position += m_front * distance;
            break;
        case Back:
            m_position -= m_front * distance;
            break;
        case Right:
            m_position += m_right * distance;
            break;
        case Left:
            m_position -= m_right * distance;
            break;
        case Up:
            m_position += m_up * distance;
            break;
        case Down:
            m_position -= m_up * distance;
            break;
    }
}

void Camera::rotate(Camera::Axis a, float angle) {
    float an = angle * 3.141592 / 720;
    float s = sin(an), c = cos(an);
    glm::fquat q;
    glm::vec3 axis;
    switch(a) {
        case X:
            axis = glm::vec3(1, 0, 0) * s;
            break;
        case Y:
            axis = glm::vec3(0, 1, 0) * s;
            break;
        case Z:
            axis = glm::vec3(0, 0, -1) * s;
            break;
    }
    q = glm::fquat(c, axis.x, axis.y, axis.z);
    m_orientation = q * m_orientation;
    m_orientation = glm::normalize(m_orientation);
    
    glm::mat4 m = glm::mat4_cast(glm::conjugate(m_orientation));
    m_right = (m * glm::vec4(glm::vec3(1, 0, 0), 1)).xyz();
    m_up = (m * glm::vec4(glm::vec3(0, 1, 0), 1)).xyz();
    m_front = (m * glm::vec4(glm::vec3(0, 0, -1), 1)).xyz();
}

glm::vec3 Camera::position() const {
    return m_position;
}

glm::mat4 Camera::mat() const {
    glm::mat4 m(glm::translate(glm::mat4(1), -m_position));
    m = glm::mat4_cast(m_orientation) * m;
    
    return m;
}
