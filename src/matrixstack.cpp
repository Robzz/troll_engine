#include <matrixstack.h>

MatrixStack::MatrixStack() :
    m_current(glm::mat4(1.f))
{
    push(m_current);
}

MatrixStack::~MatrixStack() {

}

size_t MatrixStack::size() const {
    return stack::size() + 1;
}

bool MatrixStack::empty() const {
    return size() == 1;
} 

glm::mat4 const& MatrixStack::top() const {
    return m_current;
}

void MatrixStack::push(glm::mat4 const& m) {
    glm::mat4 m2;
    m2[0] = glm::normalize(m_current[0]);
    m2[1] = glm::normalize(m_current[1]);
    m2[2] = glm::normalize(m_current[2]);
    m2[3] = m[3];
    m_current = m * m2;
    stack::push(m_current);
}

void MatrixStack::pop() {
    if(!stack::empty()) {
        stack::pop();
        m_current = stack::top();
    }
}
