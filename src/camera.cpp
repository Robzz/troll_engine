#include "camera.h"
#include "glm/gtc/matrix_transform.hpp"

Camera::Camera() :
    m_up(0, 1, 0),
    m_right(1, 0, 0),
    m_front(0, 0, -1),
    m_position(0, 0, 0),
    m_euler_angles(0, 0, 0)
{ }

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

glm::vec3 Camera::position() const {
    return m_position;
}

glm::mat4 Camera::mat() const {
    glm::mat4 m(glm::translate(glm::mat4(1), -m_position));
    
    return m;
}
