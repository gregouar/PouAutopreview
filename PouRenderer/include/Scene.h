#ifndef SCENE_H
#define SCENE_H

#include <string>

#include "Model.h"
#include "Dictionnary.h"

struct Material
{
    glm::vec4   albedo;
    float       metallic;
    float       roughness;

    std::string shaderType;
};

struct DirectionalLight
{
    glm::vec4   color;
    glm::vec3   direction;

    bool enableShadows;
    unsigned int shadowMapTexture;
    unsigned int shadowMapFBO;
    glm::mat4 shadowMapView;
};

struct CameraAngle
{
    float distance;
    float angleXY;
    float angleZ;
    bool  perspective;
};

class Scene
{
    public:
        Scene(const std::string &filepath, size_t width, size_t height);
        virtual ~Scene();

        bool loadFromFile(const std::string &filepath);

        bool initRendering(unsigned int brdfLUTTexture);
        //void render(CameraAngle &camAngle);

        void clear();
        void setupCamera(CameraAngle &camAngle, glm::mat4 &projection);
        void renderModel(Model *model, unsigned int defaultFBO = 0);

    protected:
        bool loadHDR(const std::string &hdrPath);
        void compileBlurShader(bool smart);
        void compileRenderToDepthShader();
        void generateBackfaceBuffers();
        void generateShadowmapBuffers();

        int bindShadowMaps(int textureIdShift);

        void renderDepthMap(DirectionalLight &light, Model *srcModel, glm::mat4 &model);
        void blurDepthMap(unsigned int srcFbo, unsigned int srcTexture);
        void renderBack(Model *srcModel, glm::mat4 &model);
        void blurBack();
        void renderFront(Model *srcModel, glm::mat4 &model);

        std::string getDefaultShaderVertexCode(bool enableShadowMaps);
        std::string getDefaultShaderFragmentCode(bool enableTextures, bool enableBackfaceLighting);
        //std::string getBackfaceShaderFragmentCode();


    private:
        Material m_material;
        bool     m_useTextures;

        glm::vec4 m_backgroundColor;
        glm::vec4 m_ambientLight;

        Shader  m_defaultShader[2],
                m_backfaceShader[2],
                m_smartBlurShader,
                m_blurShader,
                m_renderToDepthShader;


        size_t  m_width,
                m_height;

        std::string m_hdrTexturePath;
        unsigned int m_irradianceMap;
        unsigned int m_prefilterMap;
        unsigned int m_brdfLUTTexture;

        std::vector<DirectionalLight> m_lights;

        unsigned int m_backfaceCaptureRBOs[2];
        unsigned int m_backfaceCaptureFBOs[2];
        unsigned int m_backfaceTextures[2];
        unsigned int m_backfaceDepthTextures[2];

        unsigned int m_shadowMapPingPongFbo;
        unsigned int m_shadowMapPingPongTexture;

        float m_camXYAngle;
        float m_camZAngle;
};

#endif // SCENE_H
