#ifndef CAMERA_H
#define CAMERA_H

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

class Camera {
    public:
    /* Default constructor */
    Camera();
    /* Destructor */
    virtual ~Camera();

    /* Copy constructor */
    Camera(Camera const& other);
    /* Assignment operator */
    Camera& operator=(Camera const& other);

    enum Direction { Up, Down, Left, Right, Front, Back };
    enum Axis { X, Y, Z };

    void translate(Direction dir, float distance);
    void rotate(Axis a, float angle);
    /* Return the camera position in world space */
    glm::vec3 position() const;
    /* Return the world to camera transformation matrix */
    glm::mat4 mat() const;

    private:
    glm::vec3 m_up;
    glm::vec3 m_right;
    glm::vec3 m_front;
    glm::vec3 m_position;
    glm::fquat m_orientation;

    Camera(Camera&& other);
    Camera& operator=(Camera&& other);
};

#endif
