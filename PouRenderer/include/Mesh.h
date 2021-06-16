#ifndef MESH_H
#define MESH_H

#include "Shader.h"

#include <vector>

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Texture {
    unsigned int id;
    std::string type;
};


class Mesh
{
    public:
        // mesh data
        std::vector<Vertex>       m_vertices;
        std::vector<unsigned int> m_indices;
        std::vector<Texture>      m_textures;

        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
        ~Mesh();

        void draw(Shader *shader, Shader *shaderTextured, int textureIdShift);

    protected:
        void setupMesh();

    private:
        //  render data
        unsigned int m_VAO, m_VBO, m_EBO;
};

#endif // MESH_H
