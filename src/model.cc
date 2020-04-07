#include <glm/glm.hpp>

#include <model.hh>

#include <iostream>

using namespace glm;

void Model::draw(program p)
{
    for (auto mesh : _meshes)
        mesh.draw(p);
}

void Model::load_model(std::string path)
{
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_FlipUVs);
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << "\n";
        return;
    }
    _dir = path.substr(0, path.find_last_of('/'));

    process_node(scene->mRootNode, scene);
}


void Model::process_node(aiNode *node, const aiScene *scene)
{
    for (uint i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        _meshes.emplace_back(process_mesh(mesh, scene));
    }

    for (uint i = 0; i < node->mNumChildren; i++)
        process_node(node->mChildren[i], scene);
}

Mesh Model::process_mesh(aiMesh *mesh, const aiScene *scene)
{
    std::vector<Vertex> vertices;
    std::vector<uint> indices;
    std::vector<Texture> textures;

    for (uint i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        vertex.position = vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        vertex.normal = vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        vertex.uv = vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        vertex.tangent = vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
        vertex.bitangent = vec3(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);

        vertices.emplace_back(vertex);
    }

    for (uint i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (uint j = 0; j < face.mNumIndices; j++)
            indices.emplace_back(face.mIndices[j]);
    }

    return Mesh(vertices, indices, _dir);
}
