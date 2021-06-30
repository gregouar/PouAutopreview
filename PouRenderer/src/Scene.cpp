#include "Scene.h"
#include "Utils.h"

#include <math.h>

#include <glm/gtc/matrix_transform.hpp>


const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;


//void renderQuad(glm::vec2 startPoint = glm::vec2(-1.0f), glm::vec2 endPoint = glm::vec2(1.0f));


unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad(/*glm::vec2 startPoint, glm::vec2 endPoint*/)
{
    if (quadVAO == 0)
    {
        float quadVertices[] = {
            // positions        // texture Coords
             /*startPoint.x,  endPoint.y,     0.0f, 0.0f, 1.0f,
             startPoint.x,  startPoint.y,   0.0f, 0.0f, 0.0f,
             endPoint.x,    endPoint.y,     0.0f, 1.0f, 1.0f,
             endPoint.x,    startPoint.y,   0.0f, 1.0f, 0.0f,*/
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

Scene::Scene(const std::string &filepath, size_t width, size_t height)
{
    m_material.albedo = glm::vec4(0.5,0.5,0.5,1.0);
    m_material.metallic = 0;
    m_material.roughness = .5;
    m_material.shaderType = "Default";

    m_width = width;
    m_height = height;

    m_irradianceMap = -1;
    m_prefilterMap = -1;

    m_useTextures = true;

    this->loadFromFile(filepath);
}

Scene::~Scene()
{
    //dtor
}

bool Scene::loadFromFile(const std::string &filepath)
{
    std::cout<<"Load Scene from file: "<<filepath<<std::endl;

    std::ifstream dataFile(filepath);

    std::string fieldStr, line;
    while(dataFile >> fieldStr)
    {
        if (fieldStr == "HDR:") {
            std::getline(dataFile, line);
            m_hdrTexturePath = removeWhite(line);
        } else if(fieldStr == "Background:") {
            m_backgroundColor = readVec4(dataFile);
        } else if(fieldStr == "Ambient:") {
            m_ambientLight = readVec4(dataFile);
        } else if(fieldStr == "Light:") {
            m_lights.push_back({});
            m_lights.back().color = readVec4(dataFile);
            m_lights.back().direction = readVec3(dataFile);
            m_lights.back().enableShadows = readBool(dataFile);
        } else if(fieldStr == "Material:") {
            m_material.albedo = readVec4(dataFile);

            dataFile >> fieldStr;
            m_material.metallic = std::stof(fieldStr);

            dataFile >> fieldStr;
            m_material.roughness = std::stof(fieldStr);
        } else if(fieldStr == "UseTextures:") {
            m_useTextures = readBool(dataFile);
        }
    }

    return (true);
}

bool Scene::initRendering(unsigned int brdfLUTTexture)
{
    std::string vertexShaderSource = this->getDefaultShaderVertexCode(true);

    std::string fragmentShaderSource;

    for(int i = 0 ; i < 2 ; ++i)
    {
        fragmentShaderSource = this->getDefaultShaderFragmentCode(i, true);
        m_defaultShader[i].compile(vertexShaderSource, fragmentShaderSource);

        fragmentShaderSource = this->getDefaultShaderFragmentCode(i, false);
        m_backfaceShader[i].compile(vertexShaderSource, fragmentShaderSource);


        m_backfaceShader[i].use();
        m_backfaceShader[i].setVec4("ambientLight", m_ambientLight);

        m_backfaceShader[i].setVec4("albedo",m_material.albedo);
        m_backfaceShader[i].setFloat("metallic", m_material.metallic);
        m_backfaceShader[i].setFloat("roughness", m_material.roughness);

        m_backfaceShader[i].setInt("irradianceMap", 0);
        m_backfaceShader[i].setInt("prefilterMap", 1);
        m_backfaceShader[i].setInt("brdfLUT", 2);


        m_defaultShader[i].use();
        m_defaultShader[i].setVec4("ambientLight", m_ambientLight);

        m_defaultShader[i].setVec4("albedo",m_material.albedo);
        m_defaultShader[i].setFloat("metallic", m_material.metallic);
        m_defaultShader[i].setFloat("roughness", m_material.roughness);

        m_defaultShader[i].setInt("irradianceMap", 0);
        m_defaultShader[i].setInt("prefilterMap", 1);
        m_defaultShader[i].setInt("brdfLUT", 2);
        m_defaultShader[i].setInt("backfaceMap", 3);
        m_defaultShader[i].setInt("backfaceDepth", 4);
    }

    this->compileRenderToDepthShader();
    this->generateBackfaceBuffers();
    this->generateShadowmapBuffers();
    this->loadHDR(m_hdrTexturePath);
    this->compileBlurShader(true);
    this->compileBlurShader(false);
    this->compileUIShader();

    int j = 0;
    for(auto light : m_lights)
    if(light.enableShadows)
    {
        for(int i = 0 ; i < 2 ; ++i)
        {
            m_backfaceShader[i].use();
            m_backfaceShader[i].setMat4("lightView"+std::to_string(j),light.shadowMapView);
            m_backfaceShader[i].setInt("shadowmap"+std::to_string(j),3+j);

            m_defaultShader[i].use();
            m_defaultShader[i].setMat4("lightView"+std::to_string(j),light.shadowMapView);
            m_defaultShader[i].setInt("shadowmap"+std::to_string(j),5+j);
        }
        j++;
    }

    m_brdfLUTTexture = brdfLUTTexture;

    return (true);
}


void Scene::generateBackfaceBuffers()
{
    glGenFramebuffers(2, m_backfaceCaptureFBOs);
    glGenTextures(2, m_backfaceTextures);
    glGenTextures(2, m_backfaceDepthTextures);
    glGenRenderbuffers(2, m_backfaceCaptureRBOs);

    for(int i = 0 ; i < 2 ; ++i)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_backfaceCaptureFBOs[i]);

        glBindTexture(GL_TEXTURE_2D, m_backfaceTextures[i]);
        //glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, m_width, m_height, 0,  GL_RED, GL_FLOAT, 0);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_width, m_height, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_backfaceTextures[i], 0);

        glBindTexture(GL_TEXTURE_2D, m_backfaceDepthTextures[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, m_width, m_height, 0,  GL_RED, GL_FLOAT, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_backfaceDepthTextures[i], 0);

        unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
        glDrawBuffers(2, attachments);

        glBindRenderbuffer(GL_RENDERBUFFER, m_backfaceCaptureRBOs[i]);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_width, m_height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_backfaceCaptureRBOs[i]);
        // finally check if framebuffer is complete
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "Framebuffer not complete!" << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Scene::generateShadowmapBuffers()
{
    unsigned int rbo;

    glGenFramebuffers(1, &m_shadowMapPingPongFbo);
    glGenTextures(1, &m_shadowMapPingPongTexture);
    glGenRenderbuffers(1, &rbo);

    glBindFramebuffer(GL_FRAMEBUFFER, m_shadowMapPingPongFbo);
    glBindTexture(GL_TEXTURE_2D, m_shadowMapPingPongTexture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG32F, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_RG, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_shadowMapPingPongTexture, 0);

    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    for(auto &light : m_lights)
    if(light.enableShadows)
    {

        glGenFramebuffers(1, &light.shadowMapFBO);
        glGenTextures(1, &light.shadowMapTexture);
        glGenRenderbuffers(1, &rbo);

        glBindFramebuffer(GL_FRAMEBUFFER, light.shadowMapFBO);
        glBindTexture(GL_TEXTURE_2D, light.shadowMapTexture);


        /*glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                     SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glBindFramebuffer(GL_FRAMEBUFFER, light.shadowMapFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, light.shadowMapTexture, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        */



        glTexImage2D(GL_TEXTURE_2D, 0, GL_RG32F, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_RG, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, light.shadowMapTexture, 0);

        /*unsigned int attachments[1] = { GL_COLOR_ATTACHMENT0 };
        glDrawBuffers(1, attachments);*/



        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);


        if(glm::abs(light.direction.z) == 1 &&
           light.direction.x == 0 &&
           light.direction.y == 0)
        {
            light.shadowMapView = glm::lookAt(-light.direction,
                                                glm::vec3( 0.0f, 0.0f,  0.0f),
                                                glm::vec3( 0.0f, 1.0f,  0.0f));
        } else {
            light.shadowMapView = glm::lookAt(-light.direction,
                                                glm::vec3( 0.0f, 0.0f,  0.0f),
                                                glm::vec3( 0.0f, 0.0f,  1.0f));
        }



        /*std::cout<<light.direction.x<<" "<<light.direction.y<<" "<<light.direction.z<<" "<<std::endl<<std::endl;

        std::cout<<light.shadowMapView[0][0]<<" "<<light.shadowMapView[0][1]<<" "<<light.shadowMapView[0][2]<<" "<<std::endl;
        std::cout<<light.shadowMapView[1][0]<<" "<<light.shadowMapView[1][1]<<" "<<light.shadowMapView[1][2]<<" "<<std::endl;
        std::cout<<light.shadowMapView[2][0]<<" "<<light.shadowMapView[2][1]<<" "<<light.shadowMapView[2][2]<<" "<<std::endl;*/
    }


    auto orthoMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f,1.0f,-0.1f));
  //   orthoMatrix = glm::translate(orthoMatrix, glm::vec3(0.5f,0.5f,0.0f));

    m_renderToDepthShader.use();
    m_renderToDepthShader.setMat4("projection", orthoMatrix);

    for(int i = 0 ; i< 2 ; ++i)
    {
        m_backfaceShader[i].use();
        m_backfaceShader[i].setMat4("lightProjection", orthoMatrix);

        m_defaultShader[i].use();
        m_defaultShader[i].setMat4("lightProjection", orthoMatrix);
    }
}


void Scene::clear()
{
    glViewport(0, 0, m_width, m_height);

    glClearDepth(1.0);
    glClearColor(m_backgroundColor.x, m_backgroundColor.y, m_backgroundColor.z, m_backgroundColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Scene::setupCamera(CameraAngle &camAngle, glm::mat4 &projection)
{
    /**glm::vec3 camPos(camAngle.distance*cos(camAngle.angleXY*3.14f/180.0f)*cos(camAngle.angleZ*3.14f/180.0f),
                camAngle.distance*sin(camAngle.angleXY*3.14f/180.0f)*cos(camAngle.angleZ*3.14f/180.0f),
                camAngle.distance*sin(camAngle.angleZ*3.14f/180.0f));**/

    /**glm::vec3 camPos(0,
                -camAngle.distance*cos(camAngle.angleZ*3.14f/180.0f),
                camAngle.distance*sin(camAngle.angleZ*3.14f/180.0f));*/
    glm::vec3 camPos(camAngle.distance,
                0,
                0);


    glm::mat4 view = glm::mat4(1.0f);
    view = glm::lookAt(camPos, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 1.0));

    for(int i = 0 ; i < 2 ; ++i)
    {
        m_backfaceShader[i].use();
        m_backfaceShader[i].setMat4("projection", projection);
        m_backfaceShader[i].setMat4("view", view);
        m_backfaceShader[i].setVec3("camPos", camPos);

        m_defaultShader[i].use();
        m_defaultShader[i].setMat4("projection", projection);
        m_defaultShader[i].setMat4("view", view);
        m_defaultShader[i].setVec3("camPos", camPos);
    }


    m_camXYAngle = camAngle.angleXY;
    m_camZAngle = camAngle.angleZ;
}

void Scene::renderModel(TextRenderer *textRenderer, Model *srcModel, unsigned int defaultFBO)
{
    if(!srcModel)
        return;

    glm::mat4 model = glm::mat4( 1.0f);
    model = srcModel->getNormalization();
    model = glm::rotate(glm::mat4( 1.0f), m_camXYAngle*3.14f/180.0f, glm::vec3(0,0,1))*model;
    model = glm::rotate(glm::mat4( 1.0f), m_camZAngle*3.14f/180.0f, glm::vec3(0,1,0))*model;

    for(auto &light : m_lights)
        this->renderDepthMap(light, srcModel, model);

    if(m_material.albedo.a < 1)
    {
        this->renderBack(srcModel, model);
        this->blurBack();
    }

    glBindFramebuffer(GL_FRAMEBUFFER, defaultFBO);
    this->renderFront(srcModel, model);
    this->renderScale(textRenderer, srcModel->getScaleFactor());
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


float computeScale(float scaleFactor)
{
    int exponent = (int)(log10(scaleFactor));

    float tenPower =  pow(10.0f, exponent);

    if(scaleFactor < tenPower * 3)
    {
        return tenPower;
    }
    else if (scaleFactor < tenPower * 7.5)
    {
        return tenPower * 5;
    }
    else
    {
        return tenPower * 10;
    }
}

void Scene::renderScale(TextRenderer *textRenderer, float scaleFactor)
{
    /* should add options to personnalize size, position etc */

    float desiredScaleLength = .25f;
    float scaleWidth = .1f;

    float rescaleFactor = scaleFactor * desiredScaleLength * 0.5;
    float roundedScaleFactor = computeScale(rescaleFactor);

    m_UIshader.use();

    m_UIshader.setVec4("color", glm::vec4 (0.0f,0.0f,0.0f,1.0f));
    m_UIshader.setVec2("startPos", glm::vec2(-0.99f, -.99f));
    m_UIshader.setVec2("size", glm::vec2(desiredScaleLength*roundedScaleFactor/rescaleFactor, scaleWidth));

    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

    renderQuad();

    std::string scaleText;

    if(roundedScaleFactor < 10)
        scaleText = std::to_string((int)(roundedScaleFactor))+"mm";
    else //if(roundedScaleFactor < 100)
        scaleText = std::to_string((int)(roundedScaleFactor/10.0f))+"cm";

    textRenderer->renderText(scaleText,-1.0f+0.02,-1.0f+0.05,1.0f,glm::vec3(1.0f));
}

/*void Scene::render(CameraAngle &camAngle)
{
    glClearDepth(1.0);
    glClearColor(m_backgroundColor.x, m_backgroundColor.y, m_backgroundColor.z, m_backgroundColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(!m_model)
        return;

     glm::vec3 camPos(0,
                -m_camAngle.distance*cos(m_camAngle.angleZ*3.14f/180.0f),
                m_camAngle.distance*sin(m_camAngle.angleZ*3.14f/180.0f));

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::lookAt(camPos, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 1.0));

    this->renderBack();
    this->renderFront();
}*/


void Scene::renderDepthMap(DirectionalLight &light, Model *srcModel, glm::mat4 &model)
{
    if(!light.enableShadows)
        return;

    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);

    glBindFramebuffer(GL_FRAMEBUFFER, light.shadowMapFBO);

    glClearColor(0,0,0,0);
    //glClearDepth(0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    m_renderToDepthShader.use();
    m_renderToDepthShader.setMat4("view", light.shadowMapView);


    glm::mat4 planeModel = glm::mat4(1);
    planeModel = glm::translate(planeModel, glm::vec3(0,0,-.5));
    m_renderToDepthShader.setMat4("model", planeModel);
    renderQuad();



    m_renderToDepthShader.setMat4("model", model);
    srcModel->draw(&m_renderToDepthShader, &m_renderToDepthShader, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    this->blurDepthMap(light.shadowMapFBO, light.shadowMapTexture);
}

void Scene::blurDepthMap(unsigned int srcFbo, unsigned int srcTexture)
{
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);

    bool horizontal = true;
    unsigned int amount = 8;
    m_blurShader.use();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    //glDisable(GL_MULTISAMPLE);

    unsigned int blurFbos[2] = {srcFbo, m_shadowMapPingPongFbo};
    unsigned int blurTextures[2] = {srcTexture, m_shadowMapPingPongTexture};

    for (unsigned int i = 0; i < amount; i++)
    {
        m_blurShader.setFloat("offset", ((int)(i/2)+1)*0.5);

        glBindFramebuffer(GL_FRAMEBUFFER, blurFbos[horizontal]);
        m_blurShader.setInt("horizontal", horizontal);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, blurTextures[!horizontal]);

        renderQuad();
        horizontal = !horizontal;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}


int Scene::bindShadowMaps(int textureIdShift)
{
    for(auto &light : m_lights)
    if(light.enableShadows)
    {
        glActiveTexture(GL_TEXTURE0+textureIdShift);
        glBindTexture(GL_TEXTURE_2D, light.shadowMapTexture);

        textureIdShift++;
    }

    return textureIdShift;
}


void Scene::renderBack(Model *srcModel, glm::mat4 &model)
{
    glViewport(0, 0, m_width, m_height);

    for(int i = 0 ; i < 2 ; ++i)
    {
        m_backfaceShader[i].use();
        m_backfaceShader[i].setMat4("model", model);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, m_backfaceCaptureFBOs[0]);

    glClearColor(0,0,0,0);
    //glClearDepth(0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    //glDepthFunc(GL_GREATER);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_irradianceMap);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_prefilterMap);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, m_brdfLUTTexture);

    int textureIdShift = 3;
    textureIdShift += this->bindShadowMaps(textureIdShift);

    srcModel->draw(&m_backfaceShader[0], &m_backfaceShader[m_useTextures], textureIdShift);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}



void Scene::blurBack()
{
    glViewport(0, 0, m_width, m_height);

    bool horizontal = true;
    unsigned int amount = 8;
    m_smartBlurShader.use();
    m_smartBlurShader.setFloat("smartThresold", 1.0f);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    //glDisable(GL_MULTISAMPLE);

    for (unsigned int i = 0; i < amount; i++)
    {
        m_smartBlurShader.setFloat("offset", ((int)(i/2)+1)*0.5);

        glBindFramebuffer(GL_FRAMEBUFFER, m_backfaceCaptureFBOs[horizontal]);
        m_smartBlurShader.setInt("horizontal", horizontal);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_backfaceTextures[!horizontal]);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, m_backfaceDepthTextures[!horizontal]);

        renderQuad();
        horizontal = !horizontal;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}



void Scene::renderFront(Model *srcModel, glm::mat4 &model)
{
    glViewport(0, 0, m_width, m_height);

    for(int i = 0 ; i < 2 ; ++i)
    {
        m_defaultShader[i].use();
        m_defaultShader[i].setMat4("model", model);
    }

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_irradianceMap);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_prefilterMap);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, m_brdfLUTTexture);

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, m_backfaceTextures[0]);

    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, m_backfaceDepthTextures[0]);


    int textureIdShift = 5;
    textureIdShift += this->bindShadowMaps(textureIdShift);

    srcModel->draw(&m_defaultShader[0], &m_defaultShader[m_useTextures], textureIdShift);
}



