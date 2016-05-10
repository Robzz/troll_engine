#ifndef SCENE_IMPORTER_H
#define SCENE_IMPORTER_H

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <string>

#include "utility.h"

namespace Engine {

/**
  * \class SceneImporter
  * \brief This class handles the import of scene geometry from files on the disk or in
  * memory.
  * The SceneImporter can also perform postprocessing on the scene file. 
  */
class SceneImporter {
    public:
        /**
          * \enum PostProcess
          * \brief Available postprocessing steps.
          */
        enum class PostProcess : unsigned int {
            /** Removes duplicate vertices from the mesh. Returns indexed geometry.
             *  If not specified, the returned geometry will be suitable for array
             *  rendering. */
            JoinVertices = aiProcess_JoinIdenticalVertices,
            /** Triangulates all polygons with more than 3 faces. Will not affect points and lines. */
            Triangulate = aiProcess_Triangulate
        };

        SceneImporter();
        virtual ~SceneImporter();

        void readFile(std::string const& file, PostProcess pp);

        /* No copy or move */
        SceneImporter(SceneImporter const& other) = delete;
        SceneImporter& operator=(SceneImporter const& other) = delete;
        SceneImporter(SceneImporter&& other) = delete;
        SceneImporter& operator=(SceneImporter&& other) = delete;

    private:
        Assimp::Importer m_assimp;
};

namespace traits {
    template <>
    struct enable_bitmask_operators<SceneImporter::PostProcess> {
        static bool constexpr enable = true;
    };
}

} // namespace Engine

#endif
