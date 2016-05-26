/**
  * \file camera.h
  * \brief Camera class definition
  * \author R.Chavignat
  */
#ifndef CAMERA_H
#define CAMERA_H

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

/**
  * \class Camera
  * \brief Represents a camera in a 3D scene.
  */
class Camera {
    public:
    /**
      * \brief Default constructor.
      * Construct a camera located at the origin and looking
      * on the negative Z direction.
      */
    Camera();
    /**
      * \brief Destructor
      */
    virtual ~Camera();

    /**
      * \brief Copy constructor
      */
    Camera(Camera const& other);
    /**
      * \brief Assignment operator.
      */
    Camera& operator=(Camera const& other);

    /**
      * \enum Direction
      * \brief Represent the axis aligned directions in 3D.
      */
    enum Direction { Up, Down, Left, Right, Front, Back };
    /**
      * \enum Axis 
      * \brief Represent the different axes in 3D.
      */
    enum Axis { X, Y, Z };

    /**
      * \brief Translate the camera along the specified axis.
      */
    void translate(Direction dir, float distance);
    /**
      * \brief Rotate the camera around the specified axis.
      */
    void rotate(Axis a, float angle);
    /**
      * \brief Return the world space camera position
      */
    glm::vec3 position() const;
    /**
      * \brief Return the world to camera transformation matrix.
      */
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
