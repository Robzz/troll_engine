#include "planet.h"
#include "glm/gtc/matrix_transform.hpp"

std::chrono::time_point<std::chrono::high_resolution_clock> Planet::s_start = std::chrono::high_resolution_clock::now();;

const float Planet::c_rotK = 2 * 3.141592 / 24;
const float Planet::c_revK = 2 * 3.141592 / 365.25 / 5;

Planet::Planet(float radius, float distance, float rotPeriod, float revPeriod,
               Program& p, VBO& indices, VAO& vao, int nVertices, Texture const& tex) :
    IndexedObject(glm::mat4(1), p, indices, vao, nVertices, tex),
    m_radius(radius),
    m_distance(distance),
    m_rotSpeed(rotPeriod * c_rotK),
    m_revSpeed(revPeriod * c_revK),
    m_tex(tex)
{ }

Planet::~Planet() { };

void Planet::updateWorldTransform(float t) {
    glm::mat4 rot_self(glm::rotate(glm::mat4(1), t * m_rotSpeed, glm::vec3(0, 1, 0)));
    glm::mat4 trans(glm::translate(glm::mat4(1), glm::vec3(m_distance, 0, 0)));
    //glm::mat4 rot_sun(glm::rotate(glm::mat4(1), t*m_revSpeed, glm::vec3(0,1,0)));
    glm::mat4 rot_sun(1);
    glm::mat4 scale(glm::scale(glm::mat4(1), glm::vec3(m_radius)));
    set_transform(rot_sun * trans * rot_self * scale);
}

void Planet::draw(glm::mat4 const& m) {
    std::chrono::duration<float> t = (std::chrono::high_resolution_clock::now() - s_start);
    updateWorldTransform(t.count());
    IndexedObject::draw(m);
}
