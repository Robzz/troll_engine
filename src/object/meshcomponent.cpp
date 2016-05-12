#include "object/meshcomponent.h"

namespace Engine {

MeshComponent::MeshComponent() : ComponentBase() { }
MeshComponent::~MeshComponent() { }

MeshComponent::MeshComponent(MeshComponent const& other) : ComponentBase() { }
MeshComponent::MeshComponent(MeshComponent&& other) : ComponentBase() { }
MeshComponent& MeshComponent::operator=(MeshComponent const& other) { return *this; }
MeshComponent& MeshComponent::operator=(MeshComponent&& other) { return *this; }

} // namespace Engine
