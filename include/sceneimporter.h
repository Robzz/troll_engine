/**
  * \file include/sceneimporter.h
  * \brief Contains the definition of the SceneImporter class.
  * \author R.Chavignat
  */
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
            /** Quick hack that should go away ASAP */
            RemoveComponents = aiProcess_RemoveComponent,
            /** Optimize the scene hierarchy */
            OptimizeGraph = aiProcess_OptimizeGraph,
            /** Merge meshes together, if possible */
            OptimizeMeshes = aiProcess_OptimizeMeshes,
            /** Generate normals if the mesh has none */
            GenerateNormals = aiProcess_GenNormals,
            /** Flip the winding order of faces */
            FlipWindingOrder = aiProcess_FlipWindingOrder
        };

        /**
         * @brief Constructor.
         */
        SceneImporter();

        /**
         * @brief Destructor.
         */
        virtual ~SceneImporter();

        /**
         * @brief Load a file into the importer.
         *
         * @param file Path of the file
         * @param pp Postprocessing flags
         */
        void readFile(std::string const& file, PostProcess pp = PostProcess::None);

        /**
         * @brief Return a list of the meshes in the loaded scene.
         *
         * @return List of the meshes in the loaded scene
         */
        std::vector<const aiMesh*> meshes() const;

        /**
         * @brief Instantiate a mesh contained in the scene.
         *
         * @param mesh aiMesh to instantiate
         *
         * @return An instance of the Mesh.
         */
        std::unique_ptr<Mesh> instantiateMesh(aiMesh const& mesh) const;

        /**
          * \fn dropComponents
          * \brief  Specify which components to remove if the RemoveComponents postprocessing
          * flag is set.
          */
        void dropComponents(int flags);

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

} // namespace Engine

#endif
