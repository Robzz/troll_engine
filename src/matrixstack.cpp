#include <matrixstack.h>

MatrixStack::MatrixStack(glm::mat4 const& rootMatrix) :
    m_current(rootMatrix)
{
    push(rootMatrix);
}

MatrixStack::~MatrixStack() {

}

size_t MatrixStack::size() const {
    return stack::size() + 1;
}

bool MatrixStack::empty() const {
    return size() == 1;
} 

glm::mat4& MatrixStack::top() {
    return m_current;
}

glm::mat4 const& MatrixStack::top() const {
    return m_current;
}

void MatrixStack::push(glm::mat4 const& m) {
    m_current *= m;
    stack::push(m_current);
}

void MatrixStack::pop() {
    if(!stack::empty()) {
        stack::pop();
        m_current = stack::top();
    }
}
