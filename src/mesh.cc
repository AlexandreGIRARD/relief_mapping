#include "mesh.hh"

#include <cstddef>
#include <iostream>

Mesh::Mesh(std::vector<Vertex> &vertices, std::vector<uint> &indices, std::string path)
{
    _vertices = vertices;
    _indices = indices;

    _texture.id = 0;
    _texture.name = path + "/basecolor.jpg";
    _texture.textureId = load_texture(_texture.name.c_str());

    _normal.id = 1;
    _normal.name = path + "/normal.jpg";
    _normal.textureId = load_texture(_normal.name.c_str());

    _height.id = 2;
    _height.name = path + "/height.jpg";
    _height.textureId = load_texture(_height.name.c_str());

    _occlusion.id = 3;
    _occlusion.name = path + "/ambientOcclusion.jpg";
    _occlusion.textureId = load_texture(_occlusion.name.c_str());

    setup_mesh();
}

void Mesh::setup_mesh()
{
    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_VBO);
    glGenBuffers(1, &_EBO);

    glBindVertexArray(_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), &_vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(uint), &_indices[0], GL_STATIC_DRAW);

    // Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    // Normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    // UVs
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

    // Tangent
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

    // Bitangent
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));

    glBindVertexArray(0);
}

void Mesh::draw(program p)
{
    // Bind textures
    glActiveTexture(GL_TEXTURE0 + _texture.id);
    glBindTexture(GL_TEXTURE_2D, _texture.textureId);
    p.addUniformTexture(_texture.id, "mtl.diffuse");

    glActiveTexture(GL_TEXTURE0 + _normal.id);
    glBindTexture(GL_TEXTURE_2D, _normal.textureId);
    p.addUniformTexture(_normal.id, "mtl.normal");

    glActiveTexture(GL_TEXTURE0 + _height.id);
    glBindTexture(GL_TEXTURE_2D, _height.textureId);
    p.addUniformTexture(_height.id, "mtl.depth");

    glActiveTexture(GL_TEXTURE0 + _occlusion.id);
    glBindTexture(GL_TEXTURE_2D, _occlusion.textureId);
    p.addUniformTexture(_occlusion.id, "mtl.occlusion");

    // Draw mesh
    glBindVertexArray(_VAO);
    glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

uint Mesh::load_texture(const char *path)
{
    uint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    // Texture parameter -> Bilinear and repeat
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nbChannels;
    unsigned char *data = stbi_load(path, &width, &height, &nbChannels, 0);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    return tex;
}
