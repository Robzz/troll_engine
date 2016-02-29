#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/orthonormalize.hpp>

namespace Engine {

// TODO : move this
enum class Direction { Up, Down, Left, Right, Front, Back };
enum class Axis { X, Y, Z };

Direction operator-(Direction d);

template <class O>
/**
  * \class TransformBase
  * \brief Abstract base class for transforms.
  */
class TransformBase {
    public:
    /* Constructors */
    TransformBase();
    TransformBase(glm::vec3 position, O orientation = O());
    TransformBase(TransformBase const& other);
    TransformBase(TransformBase&& other);

    /* Destructor */
    virtual ~TransformBase();

    /* Assignment operators */
    TransformBase& operator=(TransformBase const& other);
    TransformBase& operator=(TransformBase&& other);

    /* Getters / Setters */
    glm::vec3 position() const;
    O orientation() const;
    void set_position(glm::vec3 const& v);
    void set_orientation(O const& o);

    virtual glm::vec3 x() const;
    virtual glm::vec3 y() const;
    virtual glm::vec3 z() const;
    void translate(glm::vec3 const& v);

    virtual void translate_local(Direction dir, float distance);

    virtual void translate_local(glm::vec3 const& v);

    /**
      * \fn rotate
      * \brief Rotate the transform.
      * \param axis Axis to rotate around.
      * \param angle Angle to rotate by, in radians.
      */
    virtual void rotate(glm::vec3 const& axis, float angle) = 0;

    /**
      * \fn rotate_local
      * \brief Rotate the transform in its local space.
      * \param axis Axis to rotate around.
      * \param angle Angle to rotate by, in radians.
      */
    virtual void rotate_local(Axis axis, float angle);

    virtual glm::mat4 matrix() const = 0;

    protected:
        glm::vec3 m_position;
        O m_orientation;
};

class TransformMat : public TransformBase<glm::mat3> {
    public:
    /* Constructors */
    TransformMat();

    TransformMat(glm::vec3 position, glm::mat3 orientation);

    /* Destructor */
    virtual ~TransformMat();

    virtual glm::vec3 x() const;
    virtual glm::vec3 y() const;
    virtual glm::vec3 z() const;
    virtual void rotate(glm::vec3 const& axis, float angle);

    virtual glm::mat4 matrix() const;
};

class TransformEuler : public TransformBase<glm::vec3> {
    public:
    /* Constructors */
    TransformEuler();

    TransformEuler(glm::vec3 position, glm::vec3 orientation);

    /* Destructor */
    virtual ~TransformEuler();

    virtual glm::vec3 x() const;
    virtual glm::vec3 y() const;
    virtual glm::vec3 z() const;

    virtual void rotate(glm::vec3 const& axis, float angle);
    virtual void rotate_local(Axis axis, float angle);

    virtual glm::mat4 matrix() const;
};

TransformMat   euler_to_matrix(TransformEuler const& t);
TransformEuler matrix_to_euler(TransformMat const& t);

#include "transform.inl"

} // namespace Engine

#endif
