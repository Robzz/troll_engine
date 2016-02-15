#ifndef MATRIXSTACK_H
#define MATRIXSTACK_H

#include <stack>
#include "glm/glm.hpp"

/* Stack of matrices to represent transformations relative to one another.
 * Unlike a std::stack, this will not let you pop the last matrix, so that
 * there always is an identity matrix at the bottom of the stack. */
class MatrixStack : std::stack<glm::mat4> {
    public:
    /* Default constructor
     * Construct a new MatrixStack with the identity matrix. */
    explicit MatrixStack(glm::mat4 const& rootMatrix = glm::mat4(1.f));
    /* Destructor */
    virtual ~MatrixStack();

    /* Overloads */
    size_t size() const;
    bool empty() const;
    glm::mat4& top();
    glm::mat4 const& top() const;
    void push(glm::mat4 const& m);
    void pop();


    /* Copy constructor */
    //MatrixStack(MatrixStack const& other);
    /* Assignment operator */
    //void operator=(MatrixStack const& other);

    private:
        glm::mat4 m_current;
};

#endif
