#include "sceneimporter.h"

namespace Engine {

SceneImporter::SceneImporter() : 
    m_assimp()
{ }

SceneImporter::~SceneImporter() { }

void SceneImporter::readFile(std::string const& file, SceneImporter::PostProcess pp) {
    m_assimp.ReadFile(file, static_cast<unsigned int>(pp) & aiProcess_Triangulate);
}

std::vector<const aiMesh*> SceneImporter::meshes() const {
    std::vector<const aiMesh*> vec;
    const aiScene* scene = m_assimp.GetScene();
    for(unsigned int i = 0 ; i != scene->mNumMeshes ; ++i) {
        vec.push_back(scene->mMeshes[i]);
    }
    return vec;
}

std::unique_ptr<Mesh> SceneImporter::instantiateMesh(aiMesh const& mesh) const {
    MeshBuilder b;
    std::vector<glm::vec3> vertices, normals, uvs_cubemap;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec4> colors;
    std::vector<unsigned char> index_byte;
    std::vector<unsigned short> index_short;
    std::vector<unsigned int> index_int;
    bool hasNormals = mesh.HasNormals(), hasUVs = mesh.HasTextureCoords(0),
         hasColors = mesh.HasVertexColors(0), hasIndices = (mesh.mNumFaces != mesh.mNumVertices*3);
    size_t index_size = hasNormals ? (((mesh.mNumFaces*3) <= 255) ? 1 :
                                      ((mesh.mNumFaces*3) <= 65535) ? 2 : 4) : 0;
    for(unsigned int i = 0 ; i != mesh.mNumVertices ; ++i) {
        aiVector3D* v = mesh.mVertices + i;
        vertices.push_back(glm::vec3(v->x, v->y, v->z));
        if(hasNormals) {
            v = mesh.mNormals + i;
            normals.push_back(glm::vec3(v->x, v->y, v->z));
        }
        if(hasColors) {
            aiColor4D* c = mesh.mColors[0] + i;
            colors.push_back(glm::vec4(c->r, c->g, c->b, c->a));
        }
        if(hasUVs) {
            v = mesh.mTextureCoords[0] + i;
            uvs.push_back(glm::vec2(v->x, v->y));
        }
    }
    if(hasIndices) {
        std::function<void(std::vector<unsigned char>&, std::vector<unsigned short>&, std::vector<unsigned int>&, aiFace*)> f =
                ((index_size) == 1) ? &readIndex<1> :
                ((index_size) == 2) ? &readIndex<2> : &readIndex<4>;
        for(unsigned int i = 0 ; i != mesh.mNumFaces ; ++i) {
            f(index_byte, index_short, index_int, mesh.mFaces + i);
        }
    }
    b.vertices(std::move(vertices));
    if(hasNormals)
        b.normals(std::move(normals));
    if(hasColors)
        b.colors(std::move(colors));
    if(hasIndices) {
        ((index_size) == 1) ? b.faces<unsigned char>(std::move(index_byte)) :
        ((index_size) == 2) ? b.faces<unsigned short>(std::move(index_short)) :
                              b.faces<unsigned int>(std::move(index_int));
    }
    if(hasUVs)
        b.uvs(std::move(uvs));

    return b.build_mesh();
}

template<>
void SceneImporter::readIndex<1>(std::vector<unsigned char>& v1, std::vector<unsigned short>& v2,
                                std::vector<unsigned int>& v3, aiFace* f) {
    v1.push_back(f->mIndices[0]);
    v1.push_back(f->mIndices[1]);
    v1.push_back(f->mIndices[2]);
}

template<>
void SceneImporter::readIndex<2>(std::vector<unsigned char>& v1, std::vector<unsigned short>& v2,
                                 std::vector<unsigned int>& v3, aiFace* f) {
    v2.push_back(f->mIndices[0]);
    v2.push_back(f->mIndices[1]);
    v2.push_back(f->mIndices[2]);
}

template<>
void SceneImporter::readIndex<4>(std::vector<unsigned char>& v1, std::vector<unsigned short>& v2,
                                 std::vector<unsigned int>& v3, aiFace* f) {
    v3.push_back(f->mIndices[0]);
    v3.push_back(f->mIndices[1]);
    v3.push_back(f->mIndices[2]);
}

} // namespace Engine
