#ifndef SCENE_IMPORTER_H
#define SCENE_IMPORTER_H

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <string>

#include "utility.h"
#include "mesh.h"

namespace Engine {

/**
  * \class SceneImporter
  * \brief This class handles the import of scene geometry from files on the disk or in
  * memory. All meshes will be automatically triangulated in the process.
  * The SceneImporter can also perform postprocessing on the scene file. 
  */
class SceneImporter {
    public:
        /**
          * \enum PostProcess
          * \brief Available postprocessing steps.
          */
        enum class PostProcess : unsigned int {
            None = 0,
            /** Removes duplicate vertices from the mesh. Returns indexed geometry.
             *  If not specified, the returned geometry will be suitable for array
             *  rendering. */
            JoinVertices = aiProcess_JoinIdenticalVertices,
        };

        SceneImporter();
        virtual ~SceneImporter();

        void readFile(std::string const& file, PostProcess pp = PostProcess::None);
        std::vector<const aiMesh*> meshes() const;
        std::unique_ptr<Mesh> instantiateMesh(aiMesh const& mesh) const;

        /* No copy or move */
        SceneImporter(SceneImporter const& other) = delete;
        SceneImporter& operator=(SceneImporter const& other) = delete;
        SceneImporter(SceneImporter&& other) = delete;
        SceneImporter& operator=(SceneImporter&& other) = delete;

    private:
        Assimp::Importer m_assimp;

        template <size_t n>
        static void readIndex(std::vector<unsigned char>& v1, std::vector<unsigned short>& v2,
                              std::vector<unsigned int>& v3, aiFace* c);
};

template<>
void SceneImporter::readIndex<1>(std::vector<unsigned char>& v1, std::vector<unsigned short>& v2,
                                std::vector<unsigned int>& v3, aiFace* c);
template<>
void SceneImporter::readIndex<2>(std::vector<unsigned char>& v1, std::vector<unsigned short>& v2,
                                 std::vector<unsigned int>& v3, aiFace* c);
template<>
void SceneImporter::readIndex<4>(std::vector<unsigned char>& v1, std::vector<unsigned short>& v2,
                                 std::vector<unsigned int>& v3, aiFace* c);

namespace traits {
    template <>
    struct enable_bitmask_operators<SceneImporter::PostProcess> {
        static bool constexpr enable = true;
    };
}

#include "sceneimporter.inl"

} // namespace Engine

#endif
