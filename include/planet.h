#ifndef PLANET_H
#define PLANET_H

#include <chrono>
#include "glm/glm.hpp"
#include "scenegraph.h"

class Planet : public IndexedObject {
    public:
    /* Default constructor */
    Planet(float radius, float distance, float rotPeriod, float revPeriod,
           Program& p, VBO& indices, VAO& vao, int nVertices, Texture const& tex);
    /* Destructor */
    virtual ~Planet();

    void updateWorldTransform(float t);
    virtual void draw(glm::mat4 const& m);

    private:
    float m_radius;
    float m_distance;
    float m_rotSpeed;
    float m_revSpeed;
    Texture const& m_tex;

    static const float c_rotK;
    static const float c_revK;

    static std::chrono::time_point<std::chrono::high_resolution_clock> s_start;
};

#endif
