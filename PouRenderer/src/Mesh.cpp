#include "Mesh.h"


Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures) :
    m_vertices(vertices),
    m_indices(indices),
    m_textures(textures),
    m_VAO(0),
    m_VBO(0),
    m_EBO(0)
{
    this->setupMesh();
}

Mesh::~Mesh()
{
    /**if(m_EBO)
        glDeleteBuffers(1, &m_EBO);
    if(m_VBO)
        glDeleteBuffers(1, &m_VBO);
    if(m_VAO)
        glDeleteVertexArrays(1, &m_VAO);**/
}


void Mesh::setupMesh()
{
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int),
                 &m_indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    glBindVertexArray(0);
}

void Mesh::draw(Shader *shader, Shader *shaderTextured, int textureIdShift)
{
    shaderTextured->use();

    // bind appropriate textures
    int diffuseNr  = 1;
    int specularNr = 1;
    int normalNr   = 1;
    int heightNr   = 1;
    for(unsigned int i = 0; i < m_textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + textureIdShift + i); // active proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
        std::string number;
        std::string name = m_textures[i].type;
        if(name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if(name == "texture_specular")
            number = std::to_string(specularNr++); // transfer unsigned int to stream
        else if(name == "texture_normal")
            number = std::to_string(normalNr++); // transfer unsigned int to stream
         else if(name == "texture_height")
            number = std::to_string(heightNr++); // transfer unsigned int to stream

        // now set the sampler to the correct texture unit
        //glUniform1i(glGetUniformLocation(shader->getId(), (name + number).c_str()), i);
        shaderTextured->setInt(name + number, textureIdShift+i);

        // and finally bind the texture
        glBindTexture(GL_TEXTURE_2D, m_textures[i].id);
    }


    if(!m_textures.empty())
    {
        shaderTextured->setInt("nbrDiffuseTextures", diffuseNr-1);
        shader = shaderTextured;
    }

    shader->use();

    // draw mesh
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // always good practice to set everything back to defaults once configured.
   // glActiveTexture(GL_TEXTURE0);
}

