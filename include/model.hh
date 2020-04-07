#ifndef MODEL_HH
#define MODEL_HH

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <program.hh>
#include <mesh.hh>

#include <string>

class Model
{
public:
    Model(std::string path)
    {
        load_model(path);
    }
    void draw(program p);

private:
    void load_model(std::string path);
    void process_node(aiNode *node, const aiScene *scene);
    Mesh process_mesh(aiMesh *mesh, const aiScene *scene);

    std::vector<Mesh> _meshes;
    std::string _dir;
};

#endif /* MODEL_HH */
