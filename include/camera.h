/**
  * \file camera.h
  * \brief Camera class definition
  * \author R.Chavignat
  */
#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "transform.h"

namespace Engine {

template <class T>
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
      * \brief Assignment operator
      */
    Camera& operator=(Camera const& other);


    /**
      * \brief Translate the camera in a certain direction.
      */
    void translate(glm::vec3 const& v);
   /**
      * \brief Translate the camera in a certain direction in
      * its local coordinate space.
      */
    void translate_local(Direction dir, float f);
    /**
      * \brief Rotate the camera around the specified axis.
      */
    void rotate(glm::vec3 const& axis, float angle);
    /**
      * \brief Rotate the camera around the specified axis
      * in its local space.
      */
    void rotate_local(Axis axis, float angle);

    /**
      * \brief Return the world to camera transformation matrix.
      */
    glm::mat4 world_to_camera() const;

    /**
      * \brief Return the world to camera transform.
      */
    T const& transform() const;

    private:
    T m_camToWorld;
};

#include "camera.inl"

} // namespace Engine

#endif
