#include "attribute.h"

namespace Engine {

AttributeArray::AttributeArray(AttributeArray::Type t) :
    m_type(t)
{
    AttributeArrayInstance<glm::vec4> a;
}

AttributeArray::~AttributeArray() { }

} // namespace Engine
