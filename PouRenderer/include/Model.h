#ifndef MODEL_H
#define MODEL_H

#include <assimp/cimport.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"

class Model
{
    public:
        Model();
        virtual ~Model();

        bool loadFromFile(const std::string &path);

        void draw(Shader *shader, Shader *shaderTextured, int textureIdShift);

        float getScaleFactor();
        const glm::mat4 &getNormalization();

    protected:
        void processNode(aiNode *node, const aiScene *scene);
        Mesh processMesh(aiMesh *mesh, const aiScene *scene);
        std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
                                             const std::string &typeName);

        void computeNormalization(const aiScene *scene);

    private:
        std::string m_directory;

        // model data
        std::vector<Mesh> m_meshes;

        bool m_firstLoadedVertex;
        glm::vec3 m_minVertex;
        glm::vec3 m_maxVertex;

        float       m_scaleFactor;
        glm::mat4   m_normalizationMatrix;
};


#endif // MODEL_H
