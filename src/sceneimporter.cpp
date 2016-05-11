#include "sceneimporter.h"

#include <assimp/scene.h>

namespace Engine {

SceneImporter::SceneImporter() : 
    m_assimp()
{ }

SceneImporter::~SceneImporter() { }

void SceneImporter::readFile(std::string const& file, SceneImporter::PostProcess pp) {
    m_assimp.ReadFile(file, static_cast<unsigned int>(pp));
}

} // namespace Engine
