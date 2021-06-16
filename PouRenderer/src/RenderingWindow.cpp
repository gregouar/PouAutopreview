#include "RenderingWindow.h"


#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

#include <iostream>

#include <glm/gtc/matrix_transform.hpp>

int saveScreenshot(const std::string &filename, const std::string &format)
{
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    int x = viewport[0];
    int y = viewport[1];
    int width = viewport[2];
    int height = viewport[3];

    if(format == "png")
    {
        char *data = (char*) malloc((size_t) (width * height * 4)); // 4 components (R, G, B, A)

        if (!data)
            return 0;

        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        glReadPixels(x, y, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);

        stbi_flip_vertically_on_write(true);
        int saved = stbi_write_png((filename+".png").c_str(), width, height, 4, data, 0);

        free(data);

        return saved;
    }

    char *data = (char*) malloc((size_t) (width * height * 3)); // 3 components (R, G, B)

    if (!data)
        return 0;

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadPixels(x, y, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);

    stbi_flip_vertically_on_write(true);
    int saved = stbi_write_jpg((filename+".jpg").c_str(), width, height, 3, data, 0);

    free(data);

    return saved;

}

/*int saveScreenshot(const std::string &filename)
{
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    int x = viewport[0];
    int y = viewport[1];
    int width = viewport[2];
    int height = viewport[3];

    char *data = (char*) malloc((size_t) (width * height * 3)); // 3 components (R, G, B)

    if (!data)
        return 0;

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadPixels(x, y, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);

    stbi_flip_vertically_on_write(true);
    int saved = stbi_write_jpg((filename+".jpg").c_str(), width, height, 3, data, 0);

    free(data);

    return saved;
}*/


RenderingWindow::RenderingWindow() :
    m_glfwWindow(nullptr),
    m_model(nullptr),
    m_curScene(nullptr),
    m_defaultScene(nullptr)
{
    m_camAngle.distance = 1.5;
    m_camAngle.angleXY = 0;
    m_camAngle.angleZ = 0;

    //m_HDRTexture = -1;
    /*m_irradianceMap = -1;
    m_prefilterMap = -1;*/
    m_brdfLUTTexture = -1;


    this->init();
}

RenderingWindow::~RenderingWindow()
{
    this->destroy();

    glfwTerminate();
}

bool RenderingWindow::init()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_SAMPLES, 16);

    return (true);
}

bool RenderingWindow::create(size_t width, size_t height, const std::string &title)
{
    this->destroy();

    m_width = width;
    m_height = height;

    m_glfwWindow = glfwCreateWindow(m_width, m_height, title.c_str(), nullptr, nullptr);

    if(!m_glfwWindow)
    {
        std::cout << "Failed to create window" << std::endl;
        return (false);
    }

    glfwMakeContextCurrent(m_glfwWindow);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return (false);
    }

    //if(!this->initRendering())
      //  return (false);

    return (true);
}

void RenderingWindow::destroy()
{
    if(m_glfwWindow)
        glfwDestroyWindow(m_glfwWindow);
    m_glfwWindow = nullptr;
}

/*void RenderingWindow::setBackgroundColor(glm::vec4 color)
{
    m_backgroundColor = color;
}

void RenderingWindow::setAmbientLight(glm::vec4 color)
{
    m_ambientLight = color;
}

void RenderingWindow::addLight(DirectionalLight &light)
{
    m_lights.push_back(light);
}*/

void RenderingWindow::setupScene(const std::string &sceneName)
{
    auto foundedScene = m_scenes.find(sceneName);
    if(foundedScene == m_scenes.end())
        m_curScene = m_defaultScene;
    else
        m_curScene = &(foundedScene->second);
}

void RenderingWindow::setupCamera(CameraAngle &camAngle)
{
    m_camAngle = camAngle;
}

void RenderingWindow::setupModel(const std::string &modelPath)
{
    if(m_model)
        delete m_model;

    m_model = new Model();

    m_model->loadFromFile(modelPath);
}

/*void RenderingWindow::setupMaterial(Material &material)
{
    m_material = material;
}*/

void RenderingWindow::loadDefaultScene(const std::string &scenePath)
{
    if(m_defaultScene)
        delete m_defaultScene;
    m_defaultScene = new Scene(scenePath, m_width, m_height);
}


void RenderingWindow::addAndLoadScene(const std::string &sceneName, const std::string &scenePath)
{
    m_scenes.insert({sceneName, Scene(scenePath, m_width, m_height)});
}


void RenderingWindow::printscreen(const std::string &filepath, const std::string &format)
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_antialiasedFBO);
    saveScreenshot(filepath, format);
}

bool RenderingWindow::update()
{
    if(m_curScene)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_renderingFBO);
        m_curScene->clear();
        m_curScene->setupCamera(m_camAngle, m_camAngle.perspective ? m_projectionMatrix : m_orthoMatrix);
       // m_curScene->setupCamera(m_camAngle, m_projectionMatrix);
        m_curScene->renderModel(m_model, m_renderingFBO);
    }

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_antialiasedFBO);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_renderingFBO);
    glDrawBuffer(GL_BACK);
    glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, m_width, m_height, GL_COLOR_BUFFER_BIT, GL_LINEAR);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_antialiasedFBO);
    glDrawBuffer(GL_BACK);
    glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, m_width, m_height, GL_COLOR_BUFFER_BIT, GL_LINEAR);

    glfwSwapBuffers(m_glfwWindow);
    glfwPollEvents();

    if(glfwGetKey(m_glfwWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_glfwWindow, true);

    if(glfwWindowShouldClose(m_glfwWindow))
        return (false);

    return (true);
}

bool RenderingWindow::initRendering()
{
    this->generateBrdflut();
    this->createRenderingFBO();

    if(m_defaultScene)
        m_defaultScene->initRendering(m_brdfLUTTexture);

    for(auto &scene : m_scenes)
        scene.second.initRendering(m_brdfLUTTexture);

    m_projectionMatrix = glm::perspective(glm::radians(45.0f), (float)m_width / (float)m_height, 0.1f, 100.0f);
    /*m_orthoMatrix = glm::ortho(-((float)m_width * 0.5f), (float)m_width * 0.5f,
                                (float)m_height * 0.5f, -((float)m_height * 0.5f),
                                -1000.0f, 1000.0f);*/
    m_orthoMatrix = glm::mat4(1.0f);
    m_orthoMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f,2.0f,-0.001f));
    //m_orthoMatrix = glm::ortho(0.0f, (float)m_width, 0.0f, (float)m_height, 0.0f, -10.0f);
    //m_orthoMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(100.0f));
    //m_orthoMatrix = glm::mat4(glOrtho(0,800,0,600,0,2));



    return (true);
}


void RenderingWindow::createRenderingFBO()
{
    unsigned int renderingRBO;
    unsigned int renderingTexture;

    glGenFramebuffers(1, &m_renderingFBO);
    glGenTextures(1, &renderingTexture);
    glGenRenderbuffers(1, &renderingRBO);

    glBindFramebuffer(GL_FRAMEBUFFER, m_renderingFBO);

    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, renderingTexture);
    ///glBindTexture(GL_TEXTURE_2D, renderingTexture);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 16, GL_RGBA, m_width, m_height, true);
    ///glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    ///glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    ///glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, renderingTexture, 0);
    ///glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderingTexture, 0);

    glBindRenderbuffer(GL_RENDERBUFFER, renderingRBO);
    glRenderbufferStorageMultisample (GL_RENDERBUFFER, 16, GL_DEPTH_COMPONENT, m_width, m_height);
    ///glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_width, m_height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderingRBO);
    // finally check if framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer not complete!" << std::endl;






    glGenFramebuffers(1, &m_antialiasedFBO);
    glGenTextures(1, &renderingTexture);
    glGenRenderbuffers(1, &renderingRBO);

    glBindFramebuffer(GL_FRAMEBUFFER, m_antialiasedFBO);

    glBindTexture(GL_TEXTURE_2D, renderingTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderingTexture, 0);

    glBindRenderbuffer(GL_RENDERBUFFER, renderingRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_width, m_height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderingRBO);
    // finally check if framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer not complete!" << std::endl;


}


