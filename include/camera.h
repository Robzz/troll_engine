#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <iostream>
#include "transform.h"

template <class T>
class Camera {
    public:
    /* Constructors */
    Camera();
    Camera(Camera const& other);

    /* Destructor */
    virtual ~Camera();

    /* Assignment operator */
    Camera& operator=(Camera const& other);

    //void look_at(glm::vec3 const& pos, glm::vec3 const& target, glm::vec3 const& up)

    void translate(glm::vec3 const& v);

    void translate_local(Direction dir, float f);

    void translate_local(Direction dir, glm::vec3 const& v);

    void rotate(glm::vec3 const& axis, float angle);

    void rotate_local(Axis axis, float angle);

    glm::mat4 world_to_camera() const;

    T const& transform() const;

    private:
    T m_camToWorld;
};

#include "camera.inl"

#endif
