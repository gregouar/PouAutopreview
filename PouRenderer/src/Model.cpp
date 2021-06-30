#include "Model.h"

#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/extended_min_max.hpp>

#include "stb/stb_image.h"


Model::Model() :
    m_firstLoadedVertex(true)
{
    //ctor
}

Model::~Model()
{
    //dtor
}


void Model::draw(Shader *shader, Shader *shaderTextured, int textureIdShift)
{
    for(unsigned int i = 0; i < m_meshes.size(); i++)
        m_meshes[i].draw(shader, shaderTextured, textureIdShift);
}


float Model::getScaleFactor()
{
    return m_scaleFactor;
}

const glm::mat4 &Model::getNormalization()
{
    return m_normalizationMatrix;
}


bool Model::loadFromFile(const std::string &filePath)
{
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return (false);
    }

    m_directory = filePath.substr(0, filePath.find_last_of('\\'));

    processNode(scene->mRootNode, scene);
    computeNormalization(scene);

    return (true);
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
    // process all the node's meshes (if any)
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        m_meshes.push_back(processMesh(mesh, scene));
    }
    // then do the same for each of its children
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}


Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        // process vertex positions, normals and texture coordinates
        glm::vec3 v;
        v.x = mesh->mVertices[i].x;
        v.y = mesh->mVertices[i].y;
        v.z = mesh->mVertices[i].z;
        vertex.Position = v;


        if(m_firstLoadedVertex)
        {
            m_firstLoadedVertex = false;
            m_maxVertex = v;
            m_minVertex = v;
        } else {
            m_maxVertex = glm::max(v, m_maxVertex);
            m_minVertex = glm::min(v, m_minVertex);
        }

        if (mesh->HasNormals())
        {
            v.x = mesh->mNormals[i].x;
            v.y = mesh->mNormals[i].y;
            v.z = mesh->mNormals[i].z;
            vertex.Normal = v;
        }

        if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);


        vertices.push_back(vertex);
    }
    // process indices
    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }


    // process materials
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    // 1. diffuse maps
    std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    // 2. specular maps
    std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    // 3. normal maps
    std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    // 4. height maps
    std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());


    return Mesh(vertices, indices, textures);
}

unsigned int loadTextureFromFile(const std::string &filePath, bool gamma)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filePath.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;


        stbi_set_flip_vertically_on_load(false);

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << filePath << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, const std::string &typeName)
{
    std::vector<Texture> textures;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        Texture texture;
        texture.id = loadTextureFromFile(m_directory+"//"+str.C_Str(), false);
        texture.type = typeName;
        ///texture.path = str;
        textures.push_back(texture);
    }
    return textures;
}

void Model::computeNormalization(const aiScene *scene)
{
    float tmp = std::max(std::max(m_maxVertex.x-m_minVertex.x,m_maxVertex.y-m_minVertex.y),m_maxVertex.z-m_minVertex.z);

    m_scaleFactor = tmp;

    glm::vec3 center = 0.5f * glm::vec3(m_maxVertex.x+m_minVertex.x,
                                        m_maxVertex.y+m_minVertex.y,
                                        m_maxVertex.z+m_minVertex.z);

    m_normalizationMatrix = glm::mat4( 1.0f);
    m_normalizationMatrix = glm::scale(m_normalizationMatrix, glm::vec3(1.0f/tmp));
    m_normalizationMatrix = glm::translate(m_normalizationMatrix, -center);
}

