#ifndef RENDERINGWINDOW_H
#define RENDERINGWINDOW_H

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glad/glad.h>

#include <string>

#include "Scene.h"

class RenderingWindow
{
    public:
        RenderingWindow();
        virtual ~RenderingWindow();

        bool create(size_t width, size_t height, const std::string &title = "My Awesome App");
        bool initRendering();
        void destroy();
        bool update();

        void loadDefaultScene(const std::string &sceneName);
        void addAndLoadScene(const std::string &sceneName, const std::string &scenePath);

        /*void setBackgroundColor(glm::vec4 color);
        void setAmbientLight(glm::vec4 color);
        void loadHDR(const std::string &hdrPath);

        void addLight(DirectionalLight &light);*/
        void setupScene(const std::string &sceneName);
        void setupCamera(CameraAngle &camAngle);
        void setupModel(const std::string &modelPath);
        /*void setupMaterial(Material &material);*/

        void printscreen(const std::string &filepath, const std::string &format = "jpg");

    protected:
        bool init();
        /*void renderScene();
        void renderBackFaces();*/

        void createRenderingFBO();
        void generateBrdflut();

        /*std::string getDefaultShaderVertexCode();
        std::string getDefaultShaderFragmentCode(bool enableBackfaceLighting);
        std::string getBackfaceShaderFragmentCode();*/

    private:
        GLFWwindow* m_glfwWindow;
        unsigned int m_renderingFBO,
                     m_antialiasedFBO;


        /*glm::vec4 m_backgroundColor;
        glm::vec4 m_ambientLight;

        Shader m_defaultShader,
                m_backfaceShader;*/

        size_t  m_width,
                m_height;


        glm::mat4 m_projectionMatrix;
        glm::mat4 m_orthoMatrix;
        /* //unsigned int m_HDRTexture;
        unsigned int m_irradianceMap;
        unsigned int m_prefilterMap;*/
        unsigned int m_brdfLUTTexture;

        CameraAngle m_camAngle;
        /*std::vector<DirectionalLight> m_lights;*/

        Model *m_model;
        //Material m_material;

        /*unsigned int m_backfaceCaptureFBO;
        unsigned int m_backfaceCaptureRBO;
        unsigned int m_backfaceTexture;
        unsigned int m_backfaceDepthTexture;*/

        Scene *m_curScene;
        Scene *m_defaultScene;
        std::unordered_map<std::string, Scene> m_scenes;
};

#endif // RENDERINGWINDOW_H
