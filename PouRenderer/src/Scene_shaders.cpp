#include "Scene.h"

#include "stb/stb_image.h"
#include "stb/stb_image_write.h"

#include <glm/gtc/matrix_transform.hpp>



// renderCube() renders a 1x1 3D cube in NDC.
// -------------------------------------------------
unsigned int cubeVAO = 0;
unsigned int cubeVBO = 0;
void renderCube()
{
    // initialize (if necessary)
    if (cubeVAO == 0)
    {
        float vertices[] = {
            // back face
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
            -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
            // front face
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
            -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
            // left face
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            // right face
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left
            // bottom face
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
            -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
            // top face
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
             1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right
             1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
            -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left
        };
        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &cubeVBO);
        // fill buffer
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // link vertex attributes
        glBindVertexArray(cubeVAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    // render Cube
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

bool Scene::loadHDR(const std::string &hdrPath)
{
    stbi_set_flip_vertically_on_load(true);

    unsigned int hdrTexture;

    int width, height, nrComponents;
    float *data = stbi_loadf(hdrPath.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        glGenTextures(1, &hdrTexture);
        glBindTexture(GL_TEXTURE_2D, hdrTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Failed to load HDR image: "<< hdrPath << std::endl;
        return (false);
    }


    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    std::string cubemapVertexShaderCode =
"#version 430 core \n"
"layout (location = 0) in vec3 aPos;"
""
"out vec3 localPos;"
""
"uniform mat4 projection;"
"uniform mat4 view;"
""
"void main()"
"{"
"    localPos = aPos;  "
"    gl_Position =  projection * view * vec4(localPos, 1.0);"
"}";


    std::string equirectToCubemapFragCode =
"#version 430 core \n"
"out vec4 FragColor;"
"in vec3 localPos;"
""
"uniform sampler2D equirectangularMap;"
""
"const vec2 invAtan = vec2(0.1591, 0.3183);"
"vec2 SampleSphericalMap(vec3 v)"
"{"
"    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));"
"    uv *= invAtan;"
"    uv += 0.5;"
"    return uv;"
"}"
""
"void main()"
"{		"
"    vec2 uv = SampleSphericalMap(normalize(localPos)); "
"    vec3 color = texture(equirectangularMap, uv).rgb;"
"    "
"    FragColor = vec4(color, 1.0);"
"}";

    Shader equirectangularToCubemapShader;
    equirectangularToCubemapShader.compile(cubemapVertexShaderCode,equirectToCubemapFragCode);


    unsigned int captureFBO, captureRBO;
    glGenFramebuffers(1, &captureFBO);
    glGenRenderbuffers(1, &captureRBO);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);


    // pbr: setup cubemap to render to and attach to framebuffer
    // ---------------------------------------------------------
    unsigned int envCubemap;
    glGenTextures(1, &envCubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
    for (unsigned int i = 0; i < 6; ++i)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // enable pre-filter mipmap sampling (combatting visible dots artifact)
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // pbr: set up projection and view matrices for capturing data onto the 6 cubemap face directions
    // ----------------------------------------------------------------------------------------------
    glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    glm::mat4 captureViews[] =
    {
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
    };

    // pbr: convert HDR equirectangular environment map to cubemap equivalent
    // ----------------------------------------------------------------------
    equirectangularToCubemapShader.use();
    equirectangularToCubemapShader.setInt("equirectangularMap", 0);
    equirectangularToCubemapShader.setMat4("projection", captureProjection);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, hdrTexture);

    glViewport(0, 0, 512, 512); // don't forget to configure the viewport to the capture dimensions.
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    for (unsigned int i = 0; i < 6; ++i)
    {
        equirectangularToCubemapShader.setMat4("view", captureViews[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderCube();
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // then let OpenGL generate mipmaps from first mip face (combatting visible dots artifact)
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);







    std::string irradianceShaderFragCode =
"#version 430 core \n"
"out vec4 FragColor;"
"in vec3 localPos;"
""
"uniform samplerCube environmentMap;"
""
"const float PI = 3.14159265359;"
""
"void main()"
"{		"
"    vec3 normal = normalize(localPos);"
"  "
"    vec3 irradiance = vec3(0.0);"
"    vec3 up    = vec3(0.0, 1.0, 0.0);"
"    vec3 right = normalize(cross(up, normal));"
"    up         = normalize(cross(normal, right));"
""
"    float sampleDelta = 0.025;"
"    float nrSamples = 0.0; "
"    for(float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta)"
"    {"
"        for(float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta)"
"        {"
"            vec3 tangentSample = vec3(sin(theta) * cos(phi),  sin(theta) * sin(phi), cos(theta));"
"            vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * normal; "
""
"            irradiance += texture(environmentMap, sampleVec).rgb * cos(theta) * sin(theta);"
"            nrSamples++;"
"        }"
"    }"
"    irradiance = PI * irradiance * (1.0 / float(nrSamples));"
"    FragColor = vec4(irradiance, 1.0);"
"}";

    Shader irradianceShader;
    irradianceShader.compile(cubemapVertexShaderCode, irradianceShaderFragCode);

     // pbr: create an irradiance cubemap, and re-scale capture FBO to irradiance scale.
    // --------------------------------------------------------------------------------
    glGenTextures(1, &m_irradianceMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_irradianceMap);
    for (unsigned int i = 0; i < 6; ++i)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);

    // pbr: solve diffuse integral by convolution to create an irradiance (cube)map.
    // -----------------------------------------------------------------------------
    irradianceShader.use();
    irradianceShader.setInt("environmentMap", 0);
    irradianceShader.setMat4("projection", captureProjection);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);

    glViewport(0, 0, 32, 32); // don't forget to configure the viewport to the capture dimensions.
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    for (unsigned int i = 0; i < 6; ++i)
    {
        irradianceShader.setMat4("view", captureViews[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_irradianceMap, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderCube();
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);




    std::string prefilterFragCode =
"#version 430 core \n"
"out vec4 FragColor;"
"in vec3 localPos;"
""
"uniform samplerCube environmentMap;"
"uniform float roughness;"
""
"const float PI = 3.14159265359;"
"float DistributionGGX(vec3 N, vec3 H, float roughness)"
"{"
"    float a = roughness*roughness;"
"    float a2 = a*a;"
"    float NdotH = max(dot(N, H), 0.0);"
"    float NdotH2 = NdotH*NdotH;"
""
"    float nom   = a2;"
"    float denom = (NdotH2 * (a2 - 1.0) + 1.0);"
"    denom = PI * denom * denom;"
""
"    return nom / denom;"
"}"
"float RadicalInverse_VdC(uint bits) "
"{"
"     bits = (bits << 16u) | (bits >> 16u);"
"     bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);"
"     bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);"
"     bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);"
"     bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);"
"     return float(bits) * 2.3283064365386963e-10; "
"}"
"vec2 Hammersley(uint i, uint N)"
"{"
"	return vec2(float(i)/float(N), RadicalInverse_VdC(i));"
"}"
"vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float roughness)"
"{"
"	float a = roughness*roughness;"
"	"
"	float phi = 2.0 * PI * Xi.x;"
"	float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a*a - 1.0) * Xi.y));"
"	float sinTheta = sqrt(1.0 - cosTheta*cosTheta);"
"	"
"	vec3 H;"
"	H.x = cos(phi) * sinTheta;"
"	H.y = sin(phi) * sinTheta;"
"	H.z = cosTheta;"
"	"
"	vec3 up          = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);"
"	vec3 tangent   = normalize(cross(up, N));"
"	vec3 bitangent = cross(N, tangent);"
"	"
"	vec3 sampleVec = tangent * H.x + bitangent * H.y + N * H.z;"
"	return normalize(sampleVec);"
"}"
"void main()"
"{		"
"    vec3 N = normalize(localPos);"
"    vec3 R = N;"
"    vec3 V = R;"
""
"    const uint SAMPLE_COUNT = 1024u;"
"    vec3 prefilteredColor = vec3(0.0);"
"    float totalWeight = 0.0;"
"    "
"    for(uint i = 0u; i < SAMPLE_COUNT; ++i)"
"    {"
"        vec2 Xi = Hammersley(i, SAMPLE_COUNT);"
"        vec3 H = ImportanceSampleGGX(Xi, N, roughness);"
"        vec3 L  = normalize(2.0 * dot(V, H) * H - V);"
""
"        float NdotL = max(dot(N, L), 0.0);"
"        if(NdotL > 0.0)"
"        {"
"            float D   = DistributionGGX(N, H, roughness);"
"            float NdotH = max(dot(N, H), 0.0);"
"            float HdotV = max(dot(H, V), 0.0);"
"            float pdf = D * NdotH / (4.0 * HdotV) + 0.0001; "
""
"            float resolution = 512.0; "
"            float saTexel  = 4.0 * PI / (6.0 * resolution * resolution);"
"            float saSample = 1.0 / (float(SAMPLE_COUNT) * pdf + 0.0001);"
""
"            float mipLevel = roughness == 0.0 ? 0.0 : 0.5 * log2(saSample / saTexel); "
"            "
"            prefilteredColor += textureLod(environmentMap, L, mipLevel).rgb * NdotL;"
"            totalWeight      += NdotL;"
"        }"
"    }"
""
"    prefilteredColor = prefilteredColor / totalWeight;"
""
"    FragColor = vec4(prefilteredColor, 1.0);"
"}";

    Shader prefilterShader;
    prefilterShader.compile(cubemapVertexShaderCode, prefilterFragCode);


    // pbr: create a pre-filter cubemap, and re-scale capture FBO to pre-filter scale.
    // --------------------------------------------------------------------------------
    glGenTextures(1, &m_prefilterMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_prefilterMap);
    for (unsigned int i = 0; i < 6; ++i)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 128, 128, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // be sure to set minification filter to mip_linear
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // generate mipmaps for the cubemap so OpenGL automatically allocates the required memory.
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    // pbr: run a quasi monte-carlo simulation on the environment lighting to create a prefilter (cube)map.
    // ----------------------------------------------------------------------------------------------------
    prefilterShader.use();
    prefilterShader.setInt("environmentMap", 0);
    prefilterShader.setMat4("projection", captureProjection);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    unsigned int maxMipLevels = 5;
    for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
    {
        // reisze framebuffer according to mip-level size.
        unsigned int mipWidth  = 128 * std::pow(0.5, mip);
        unsigned int mipHeight = 128 * std::pow(0.5, mip);
        glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
        glViewport(0, 0, mipWidth, mipHeight);

        float roughness = (float)mip / (float)(maxMipLevels - 1);
        prefilterShader.setFloat("roughness", roughness);
        for (unsigned int i = 0; i < 6; ++i)
        {
            prefilterShader.setMat4("view", captureViews[i]);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_prefilterMap, mip);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            renderCube();
        }
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return (true);
}

std::string Scene::getDefaultShaderVertexCode(bool enableShadowMaps)
{
    std::string code =
"#version 430 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aNormal;\n"
"layout (location = 2) in vec2 aTexCoords;\n"
"\n"
"out vec3 WorldPos;\n"
"out vec3 Normal;\n"
"out float zPos;\n"
"out vec2 TexCoords;";

int i = 0;
if(enableShadowMaps)
for(auto light : m_lights)
if(light.enableShadows)
{
    code +=
"uniform mat4 lightView"+std::to_string(i)+";"
"out vec3 lightPos"+std::to_string(i)+";"
";";
    i++;
}

code +=
""
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"uniform mat4 lightProjection;\n"
""
"\n"
"void main()\n"
"{\n"
"   vec4 wWorldPos = model * vec4(aPos, 1.0);\n"
"   gl_Position = projection * view * wWorldPos;\n"
"   zPos = gl_Position.z/gl_Position.w;"
"   Normal = normalize(mat3(model) * aNormal);\n"
"   TexCoords = aTexCoords; "
"   vec4 wLightPos;"
"";

i = 0;
if(enableShadowMaps)
for(auto light : m_lights)
if(light.enableShadows)
{
    code +=
"   wLightPos = lightProjection * lightView"+std::to_string(i)+" *  wWorldPos;"
//"   wLightPos.xy = wLightPos.xy * vec2("+std::to_string((float)m_width/1024.0f)+","+std::to_string((floatm_height/1024.0f)+");"
"   lightPos"+std::to_string(i)+" = wLightPos.xyz/wLightPos.w;"
//"   gl_Position = wLightPos;\n"
";";
    i++;
}

code +=
"   WorldPos = wWorldPos.xyz/wWorldPos.w;\n"
"}\0";

return code;

}

/*std::string Scene::getBackfaceShaderFragmentCode()
{
    return
"#version 430 core\n"
"out float FragColor;\n"
"in vec3 WorldPos; \n"
"in vec3 Normal; \n"
"void main()\n"
"{\n"
"   FragColor = gl_FragCoord.z;"
"}";
}*/

std::string Scene::getDefaultShaderFragmentCode(bool enableTextures, bool enableBackfaceLighting)
{
    /*return
"#version 430 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"    FragColor = vec4(1,.5,.2, 1.0);"
"}\0";*/

    std::string code = "";

    code =
"#version 430 core\n"
"out vec4 FragColor;\n";

if(!enableBackfaceLighting)
code +=
"out float FragDepth;\n";


code +=
"in vec3 WorldPos; \n"
"in vec3 Normal; \n"
"in float zPos; \n"
"in vec2 TexCoords;"
""
"uniform vec3 camPos; \n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
""
""
"uniform samplerCube irradianceMap;"
"uniform samplerCube prefilterMap;"
"uniform sampler2D brdfLUT; \n";
if(enableBackfaceLighting)
{
    code += "uniform sampler2D backfaceMap; \n";
    code += "uniform sampler2D backfaceDepth; \n";
}

code +=
""
"uniform vec4 ambientLight; \n"
""
"uniform vec4  albedo; "
"uniform float metallic; "
"uniform float roughness; "
"";

int i = 0;
for(auto light : m_lights)
if(light.enableShadows)
{
    code +=
"uniform sampler2D shadowmap"+std::to_string(i)+";"
"in vec3 lightPos"+std::to_string(i)+";"
//"uniform mat4 lightView"+std::to_string(i)+";"
";";
    i++;
}


if(enableTextures)
{
    code +=
"uniform int nbrDiffuseTextures;"
"uniform sampler2D texture_diffuse1;";
}

code +=
""
"const float PI = 3.14159265359;"
""
"const vec2 invAtan = vec2(0.1591, 0.3183);"
"vec2 SampleSphericalMap(vec3 v)"
"{"
"    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));"
"    uv *= invAtan;"
"    uv += 0.5;"
"    return uv;"
"}"
""
"vec3 fresnelSchlick(float cosTheta, vec3 F0)"
"{"
"    return F0 + (1.0 - F0) * pow(max(1.0 - cosTheta, 0.0), 5.0);"
"}"
"float DistributionGGX(vec3 N, vec3 H, float roughness)"
"{"
"    float a      = roughness*roughness;"
"    float a2     = a*a;"
"    float NdotH  = max(dot(N, H), 0.0);"
"    float NdotH2 = NdotH*NdotH;"
"	"
"    float num   = a2;"
"    float denom = (NdotH2 * (a2 - 1.0) + 1.0);"
"    denom = PI * denom * denom;"
"	"
"    return num / denom;"
"}"
""
"float GeometrySchlickGGX(float NdotV, float roughness)"
"{"
"    float r = (roughness + 1.0);"
"    float k = (r*r) / 8.0;"
""
"    float num   = NdotV;"
"    float denom = NdotV * (1.0 - k) + k;"
"	"
"    return num / denom;"
"}"
"float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)"
"{"
"    float NdotV = max(dot(N, V), 0.0);"
"    float NdotL = max(dot(N, L), 0.0);"
"    float ggx2  = GeometrySchlickGGX(NdotV, roughness);"
"    float ggx1  = GeometrySchlickGGX(NdotL, roughness);"
"	"
"    return ggx1 * ggx2;"
"}"
""
""
"vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)"
"{"
"    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(max(1.0 - cosTheta, 0.0), 5.0);"
"} "
""
""
"float linstep(float low, float high, float v){"
"    return clamp((v-low)/(high-low), 0.0, 1.0);"
"}"
"float chebyshevUpperBound(vec2 moments , float z)"
"{   "
"    float p = smoothstep(z-0.0005, z, moments.x);"
"    float variance = max(moments.y - moments.x*moments.x, -0.001);"
"    float d = z - moments.x;"
"    float p_max = linstep(0.2, 1.0, variance / (variance + d*d));"
""
"    return clamp(max(p, p_max), 0.0, 1.0);"
"}"
""
"vec3 convertToViewScreen(vec3 no){"
"   vec4 wNo = projection * view * vec4(no, 1.0);"
"   return wNo.xyz/wNo.w;"
"}"
""
"";
if(enableBackfaceLighting)
{
    code +=
"vec3 rayMarch(vec3 worldPos, vec3 ray, float stepSize)"
"{"
"   vec3 r = vec3(0);"
""
"   float nbrSteps = 500;"
"   ray = normalize(ray);"
""
"   for(float i = 1 ; i <= nbrSteps ; i += 1) {"
"       worldPos += ray * stepSize;"
"       vec2 screenPos = vec2(0.5) + convertToViewScreen(worldPos).xy/2;"
"       float sampleDepth = texture(backfaceDepth, screenPos ).r;"
"       float curDepth = length(camPos - worldPos);"
"       if(sampleDepth < curDepth && sampleDepth > curDepth - 0.01){"
"           return vec3(screenPos,i*stepSize);"
"       }"
"   }"
""
"   return r;"
"}";
/*"vec3 rayMarch(vec3 screenCoord, vec3 ray, float stepSize)"
"{"
"   vec3 r = vec3(0);"
""
"   float nbrSteps = 1000;"
""
"   ray.z = -ray.z;"
""
"   for(int i = 0 ; i < nbrSteps ; i++) {"
"       screenCoord += ray * stepSize;"
"       float sampleDepth = texture(backfaceDepth,screenCoord.xy).r;"
"       if(sampleDepth < screenCoord.z && sampleDepth > screenCoord.z - 0.05){"
"           return screenCoord;"
"       }"
"   }"
""
"   return r;"
"}";*/
}

code +=
""
""
""
"\n"
"void main()\n"
"{\n"
"   vec3 N = normalize(Normal); \n"
"   vec4 fragAlbedo = albedo;";

if(enableTextures)
{
    code +=
//"if(nbrDiffuseTextures != 0) {"
//"fragAlbedo = vec4(1,0,0,1);"
//"fragAlbedo = texture(texture_diffuse1, TexCoords);"
"fragAlbedo = texture(texture_diffuse1, vec2(TexCoords.x, 1.0-TexCoords.y));"
//"}"
"";
}

if(!enableBackfaceLighting)
{
   // code += " N = -N;";
}

code +=
"   vec3 V = normalize(camPos - WorldPos); \n"
"    vec3 R = reflect(-V, N);  ";


if(!enableBackfaceLighting)
{
    //code += " V = -V;";
}

code+=
""
"   vec3 F0 = vec3(0.04); "
"   F0 = mix(F0, vec3(fragAlbedo), metallic); "
""
"    vec3 diffuseColor = vec3(0.0);"
"    vec4 specularColor = vec4(0.0);"
"   float attenuation = 1.0;"
"   float bias = 0.0;"
"   vec2 shadowMapValue;"
"   vec3 lightPos;"
"";

i = 0;
for(auto light : m_lights)
{
    code +=
"{"
"attenuation = 1.0;"
    //"    for(int i = 0; i < 4; ++i) "
"        vec3 L = -normalize(vec3("+std::to_string(light.direction.x)+","+std::to_string(light.direction.y)+","+std::to_string(light.direction.z)+"));"//normalize(lightPositions[i] - WorldPos);"
"        vec3 H = normalize(V + L);";

    if(light.enableShadows)
    {
        code +=
"   lightPos = lightPos"+std::to_string(i)+"*0.5 + vec3(0.5);"
"   shadowMapValue = texture(shadowmap"+std::to_string(i)+", lightPos.xy).rg; "
//"   bias = max(0.001 * (1.0 - dot(N, L)), 0.0001);  "
//"   if(shadowMapValue + bias < lightPos.z){attenuation = 0.0;}"
"   attenuation = chebyshevUpperBound(shadowMapValue, lightPos.z);"
"";

        /*code += "FragColor = vec4(lightPos, attenuation); }";
        return code;*/

        i++;
    }

    code +=
//"        float distance    = length(lightPositions[i] - WorldPos);"
//"        float attenuation = 1.0 / (distance * distance);"
"        vec3 radiance     = vec3("+std::to_string(light.color.r)+","+std::to_string(light.color.g)+","+std::to_string(light.color.b)+") * "+std::to_string(light.color.a)+";"//lightColors[i];" // * attenuation;     "
"        "
"        float NDF = DistributionGGX(N, H, roughness);        "
"        float G   = GeometrySmith(N, V, L, roughness);      "
"        vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);    "
"        "
"        vec3 kS = F;"
"        vec3 kD = vec3(1.0) - kS;"
"        kD *= 1.0 - metallic;	  "
"        "
"        vec3 numerator    = NDF * G * F;"
"        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);"
"        vec3 specular     = numerator / max(denominator, 0.001);  "
"            "
"        float NdotL = max(dot(N, L), 0.0);                "
"        diffuseColor += attenuation * (kD * vec3(fragAlbedo) / PI) * radiance * NdotL;"
"        specularColor += attenuation * vec4(specular * radiance * NdotL, clamp(specular * NdotL, 0.0, 1.0));"
"    }    "
"  ";
}

//AMBIENT
/*"    vec3 kS = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness); "
"    vec3 kD = 1.0 - kS;"
"    vec3 irradiance = vec3(ambientLight)*ambientLight.a;"
"    vec3 diffuse    = irradiance * vec3(fragAlbedo);"
"    vec3 ambient    = (kD * diffuse);" // * ao;*/
code +=
"vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);"
""
"vec3 kS = F;"
"vec3 kD = 1.0 - kS;"
"kD *= 1.0 - metallic;	  "
"  "
"vec3 irradiance = vec3(ambientLight)*ambientLight.a + texture(irradianceMap, vec3(N.x, N.z, N.y)).rgb;"
"vec3 diffuse    = irradiance * vec3(fragAlbedo);"
"  "
"const float MAX_REFLECTION_LOD = 4.0;"
"vec3 prefilteredColor = textureLod(prefilterMap, vec3(R.x, R.z, R.y),  roughness * MAX_REFLECTION_LOD).rgb;   "
"vec2 envBRDF  = texture(brdfLUT, vec2(max(dot(N, V), 0.0), roughness)).rg;"
"vec3 specular = (ambientLight.rgb*ambientLight.a + prefilteredColor ) * (F * envBRDF.x + envBRDF.y);"
"  "
"   diffuseColor += kD * diffuse;"
"   specularColor += vec4(specular,   (F * envBRDF.x + envBRDF.y));"
"   vec3 finalColor = diffuseColor*fragAlbedo.a + specularColor.rgb;"
"   float finalAlpha = clamp(fragAlbedo.a+specularColor.a, 0.0, 1.0);"
""
//"    finalColor = finalColor / (finalColor + vec3(1.0));"
//"    finalColor = pow(finalColor, vec3(1.0/2.2));  "
"	";

if(enableBackfaceLighting && m_material.albedo.a < 1)
{
code +=
""
//"   vec3 refractedRay =  convertToViewScreen(refract(-V,N,1) + WorldPos) - WorldPos;" //refract(-V,N,1)
//"   vec3 colCoord = rayMarch(vec3(screenCoord, zPos), refractedRay, 1/200);"
"   vec3 refractedRay =  refract(-V,N,0.2);" //refract(-V,N,1)
""
"   vec3 perturbationArray[7] = {vec3(1,1,1),"
"           vec3(1.1,1,1), vec3(.9,1,1), vec3(1,1.1,1), vec3(1,.9,1), vec3(1,1,1.1), vec3(1,1,.9)};"
"   vec4 backfaceColor = vec4(0);"
"   float amount = 1;"
"   for(int i = 0 ; i < amount ; ++i) {"
""
""
"   vec3 colCoord = rayMarch(WorldPos, refractedRay*perturbationArray[i], 1.0/500.0);"
""
""
"   if(colCoord.z != 0){"
"       vec2 screenCoord = colCoord.xy;"
""
"       backfaceColor  +=  (texture(backfaceMap,screenCoord))/amount;"
//"    float backfaceDepth  =  texture(backfaceDepth,screenCoord).r;"
//"    backfaceColor.a *= 1.0 - clamp(50*(backfaceDepth-gl_FragCoord.z), 0, 1);"
"       backfaceColor.a += (1.0 - clamp(colCoord.z/5,0,1))/amount;"
""
"   } else {"
"       backfaceColor.rgb += (textureLod(prefilterMap, refractedRay,  roughness * MAX_REFLECTION_LOD).rgb)/amount;"
//"       backfaceColor.rgb = vec3(0,1,0);"
"       backfaceColor.a += 1/amount;"
"   }"
""
"   }"
""
"    vec2 screenCoord = vec2(gl_FragCoord.x/"+std::to_string(m_width)+",gl_FragCoord.y/"+std::to_string(m_height)+");"
"       float ratio = 0.5;"
"       backfaceColor  = backfaceColor*ratio + (texture(backfaceMap,screenCoord))*(1-ratio);"
"       float backfaceDepth  =  texture(backfaceDepth,screenCoord).r;"
"      backfaceColor.a = backfaceColor.a*ratio +  (1.0 - clamp(1*(backfaceDepth-length(WorldPos-camPos)), 0, 1))*(1-ratio);"
//"    backfaceColor.a *= 1.0 - clamp(50*(backfaceDepth-gl_FragCoord.z), 0, 1);"
""
""
"    float oldAlpha = finalAlpha;"
"    finalAlpha = finalAlpha + backfaceColor.a * (1-finalAlpha);"
"    finalColor = (finalColor * oldAlpha + backfaceColor.rgb * backfaceColor.a * (1-oldAlpha))/finalAlpha;"
""
""
//"   finalColor.rgb = vec4(projection * view * vec4(N,1)).xyz / vec4(projection * view * vec4(N,1)).w;"
//"   finalColor.rgb = vec3(0,zPos,0);"
/*"   finalColor.rgb = (convertToViewScreen(refract(-V,N,1)+WorldPos) - WorldPos) ;"*/
/*"   finalColor.rgb = convertToViewScreen(WorldPos).xyz;"
"   finalColor.rgb = vec3(.5) + finalColor.rgb/2;"
"   finalAlpha = 1;"*/
""
""
//"   finalAlpha = finalAlpha + backfaceColor.a * (1-finalAlpha);"
//"   finalColor += (1-finalAlpha) * backfaceColor.a * backfaceColor.rgb;"
""
    ;
}

code +=
"   vec4 backgroundColor = vec4("+std::to_string(m_backgroundColor.r)+","+std::to_string(m_backgroundColor.g)+","+std::to_string(m_backgroundColor.b)+","+std::to_string(m_backgroundColor.a)+");";

if(enableBackfaceLighting)
{
code +=
"    finalColor = finalColor / (finalColor + vec3(1.0));"
"    finalColor = pow(finalColor, vec3(1.0/2.2));  "
"   if(finalAlpha > 0) { "
"   backgroundColor.a = 1.0;"
"   } "
"   FragColor.a = finalAlpha + backgroundColor.a * (1 - finalAlpha);"
"   FragColor.rgb = (finalColor * finalAlpha + backgroundColor.rgb * backgroundColor.a * (1-finalAlpha))/FragColor.a;"
""
""
""
/*"   FragColor.rgb = 0*convertToViewScreen(WorldPos).xyz;"
"   FragColor.rgb = vec3(0.5);" // + 0.5*FragColor.rgb;"
"   FragColor.a = 1;";*/
"";
}
else
{
code +=
"   "
//"    FragColor = mix(vec4("+std::to_string(m_backgroundColor.r)+","+std::to_string(m_backgroundColor.g)+","+std::to_string(m_backgroundColor.b)+","+std::to_string(m_backgroundColor.a)+"),vec4(finalColor,1.0),finalAlpha);"
"   FragColor.a = finalAlpha + backgroundColor.a * (1 - finalAlpha);"
"   FragColor.rgb = (finalColor * finalAlpha + backgroundColor.rgb * backgroundColor.a * (1-finalAlpha))/FragColor.a;";
//"    FragColor = vec4(finalColor, albedo.a+specularColor.a);"
//"FragColor = vec4(WorldPos.x, WorldPos.y, WorldPos.z, 1.0);"
//"   FragColor.r = vec4( gl_FragCoord.z - texture(backfaceMap,vec2(gl_FragCoord.x,gl_FragCoord.y)).r, 0,0,1).r;"

/*if(enableBackfaceLighting)
{
code +=
"    FragColor =  texture(backfaceMap,vec2(gl_FragCoord.x/"+std::to_string(m_width)+",gl_FragCoord.y/"+std::to_string(m_height)+"));"
    ;
}*/
}



if(!enableBackfaceLighting){
code +=
"   FragDepth = length(WorldPos - camPos);";
//"   FragDepth = zPos;";
}

code +=
"}\0";

    return code;
}


void Scene::compileBlurShader(bool smart)
{
    std::string vertCode =
"#version 430 core \n"
"layout (location = 0) in vec3 aPos;"
"layout (location = 1) in vec2 aTexCoords;"
""
"out vec2 TexCoords;"
""
"void main()"
"{"
"    TexCoords = aTexCoords;"
"    gl_Position = vec4(aPos, 1.0);"
"}";

    std::string fragCode =
"#version 430 core \n";

if(smart){
fragCode +=
"out vec4 FragColor;"
"out float FragColor2;";
} else {
fragCode +=
"out vec2 FragColor;";
}

fragCode +=
"  "
"in vec2 TexCoords;"
""
"uniform sampler2D image;";

if(smart)
fragCode +=
"uniform sampler2D image2;"
"uniform float smartThresold;";

fragCode +=
"uniform float offset;"
"  "
"uniform bool horizontal;"
"uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);"
"";

if(smart)
{
fragCode +=
"vec4 sampleImg(vec2 coord, float fragDepth, float weight)"
"{"
"   vec4 r = vec4(0,0,0,0);"
"   float targetDepth = texture(image2, coord).r;"
"   if(abs(targetDepth - fragDepth) < smartThresold) {"
"       r.rgb = texture(image, coord).rgb * weight;"
"       r.a = weight;"
"   }"
"   return r;"
"}";
} else {
fragCode +=
"vec3 sampleImg(vec2 coord, float fragDepth, float weight)"
"{"
"   vec3 r = vec3(0,0,0);"
"       r.rg = texture(image, coord).rg * weight;"
"       r.b = weight;"
"   return r;"
"}";
}


fragCode +=
""
"void main()"
"{             "
"    vec2 tex_offset = offset / textureSize(image, 0);";

if(smart)
fragCode +=
"    vec4 result = vec4(texture(image, TexCoords).rgb * weight[0], weight[0]); "
"    float result2 = texture(image2, TexCoords).r; ";
else
fragCode +=
"    vec3 result = vec3(texture(image, TexCoords).rg * weight[0], weight[0]); "
"    float result2 = 0;";

//"    float result2 = texture(image2, TexCoords).r * weight[0]; "
fragCode +=
"    if(horizontal)"
"    {"
"        for(int i = 1; i < 5; ++i)"
"        {"
"           result += sampleImg(TexCoords + vec2(tex_offset.x * i, 0.0), result2, weight[i]);"
"           result += sampleImg(TexCoords - vec2(tex_offset.x * i, 0.0), result2, weight[i]);"
//"            result += texture(image, TexCoords + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];"
//"            result += texture(image, TexCoords - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];"
//"            result2 += texture(image2, TexCoords + vec2(tex_offset.x * i, 0.0)).r * weight[i];"
//"            result2 += texture(image2, TexCoords - vec2(tex_offset.x * i, 0.0)).r * weight[i];"
"        }"
"    }"
"    else"
"    {"
"        for(int i = 1; i < 5; ++i)"
"        {"
"           result += sampleImg(TexCoords + vec2(0, tex_offset.y * i), result2, weight[i]);"
"           result += sampleImg(TexCoords - vec2(0, tex_offset.y * i), result2, weight[i]);"
//"            result += texture(image, TexCoords + vec2(0.0, tex_offset.y * i)).rgb * weight[i];"
//"            result += texture(image, TexCoords - vec2(0.0, tex_offset.y * i)).rgb * weight[i];"
//"            result2 += texture(image2, TexCoords + vec2(0.0, tex_offset.y * i)).r * weight[i];"
//"            result2 += texture(image2, TexCoords - vec2(0.0, tex_offset.y * i)).r * weight[i];"
"        }"
"    }";
if(smart)
fragCode +=
"    FragColor = vec4(result.rgb/result.a, 1.0);"
"    FragColor2 = result2;";
else
fragCode +=
"    FragColor = vec2(result.rg/result.b);";

fragCode +=
"}";

    Shader &blurShader = (smart) ? m_smartBlurShader : m_blurShader;
    blurShader.compile(vertCode, fragCode);

    blurShader.use();
    blurShader.setInt("image", 0);

    if(smart)
        blurShader.setInt("image2", 1);
}


void Scene::compileRenderToDepthShader()
{
    auto vertexCode = this->getDefaultShaderVertexCode(false);

    std::string fragCode =
"#version 430 core \n"
"out vec2 squaredDepth;\n"
""
"in float zPos; \n"
""
"void main()"
"{   "
"    gl_FragDepth = zPos * 0.5 + 0.5;"
""
//"    float fd = (1-gl_FragDepth);"
"    float fd = gl_FragDepth;"
""
"    squaredDepth.r = fd;"
"    squaredDepth.g = fd * fd;"
""
"    float dx = dFdx(fd);"
"    float dy = dFdy(fd);"
"    squaredDepth.g += 0.25*(dx*dx+dy*dy);"
"}  ";

    m_renderToDepthShader.compile(vertexCode, fragCode);
}



void Scene::compileUIShader()
{

    std::string vertCode =
"#version 430 core \n"
"layout (location = 0) in vec3 aPos;"
"layout (location = 1) in vec2 aTexCoords;"
""
"uniform vec2 startPos;"
"uniform vec2 size;"
""
"out vec2 TexCoords;"
""
"void main()"
"{"
"    TexCoords = aTexCoords;"
""
"   gl_Position.xy = startPos+size*(aPos.xy + vec2(1.0))*0.5;"
"   gl_Position.zw = vec2(0.0,1.0);"
""
//"    gl_Position = vec4(aPos, 1.0);"
"}";

    std::string fragCode =
"#version 430 core \n"
"out vec4 FragColor;"
"  "
"in vec2 TexCoords;"
""
"uniform vec4 color;"
""
//"uniform sampler2D image;"
"void main()"
"{             "
"    FragColor = color;"
"}";

    m_UIshader.compile(vertCode, fragCode);
}

