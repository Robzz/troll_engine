/**
  * \file matrixstack.h
  * \brief MatrixStack class definition
  * \author R.Chavignat
  */
#ifndef MATRIXSTACK_H
#define MATRIXSTACK_H

#include <stack>
#include "glm/glm.hpp"

/**
  * \class MatrixStack
  * \brief Store a stack of matrices.
  * Stacks of matrices represent transformations relative to one another.
  * Unlike a STL stack, this will not let you pop the bottom matrix, so that
  * there always is an identity matrix at the bottom of the stack.
  */
class MatrixStack : std::stack<glm::mat4> {
    public:
    /**
      * \brief Default constructor
      * Construct a new MatrixStack containing the identity matrix.
      */
    explicit MatrixStack();
    /**
      * \brief Destructor
      */
    virtual ~MatrixStack();

    /**
      * \brief Return the size of the stack.
      * Since the MatrixStack always contain at least the bottom identity
      * matrix, this function will never return 0.
      */
    size_t size() const;
    /**
      * \brief Return true if \ref size is equal to 1.
      */
    bool empty() const;
    /**
      * \brief Return the matrix at the top of the stack
      */
    glm::mat4 const& top() const;
    /**
      * \brief Push a matrix at the top of the stack
      */
    void push(glm::mat4 const& m);
    /**
      * \brief Pop the matrix at the top of the stack.
      * If the identity matrix is the only matrix in the stack, this is a no-op.
      */
    void pop();

    private:
        glm::mat4 m_current;
};

#endif
