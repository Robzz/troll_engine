#ifndef CAMERA_H
#define CAMERA_H

#include "glm/glm.hpp"

class Camera {
    public:
    /* Default constructor */
    Camera();
    /* Destructor */
    virtual ~Camera();

    /* Move constructor */
    Camera(Camera&& other);
    /* Move-assignment operator */
    void operator=(Camera&& other);

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
    glm::vec3 m_euler_angles;

    /* Copy constructor */
    Camera(Camera const& other);
    /* Assignment operator */
    void operator=(Camera const& other);
};

#endif
