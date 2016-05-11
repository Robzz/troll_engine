#include "object/component.h"

namespace Engine {

ComponentBase::ComponentBase() { }

ComponentBase::~ComponentBase() { }

ComponentBase::ComponentBase(ComponentBase const& other) { }
ComponentBase::ComponentBase(ComponentBase&& other) { }
ComponentBase& ComponentBase::operator=(ComponentBase const& other) { return *this; }
ComponentBase& ComponentBase::operator=(ComponentBase&& other) { return *this; }

} // namespace Engine
