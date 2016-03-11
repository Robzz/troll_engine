#include "pov.h"

Viewpoint::Viewpoint(Engine::Image const& colorMap, Engine::Image const& depthMap, Engine::Image const& normalMap) :
	m_colorMap(colorMap),
	m_depthMap(depthMap),
	m_normalMap(normalMap)
{ }

Viewpoint::~Viewpoint() { }