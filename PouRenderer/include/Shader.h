#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


//#define GLM_FORCE_RADIANS
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

class Shader
{
    public:
        Shader();
        Shader(const std::string &vertexPath, const std::string &fragmentPath);

        bool loadFromFile(const std::string &vertexPath, const std::string &fragmentPath);
        bool compile(const std::string &vertexCode, const std::string &fragmentCode);

        void use();

        void setBool(const std::string &name, bool value) const;
        void setInt(const std::string &name, int value) const;
        void setFloat(const std::string &name, float value) const;
        void setVec3(const std::string &name, const glm::vec3 &vec) const;
        void setVec4(const std::string &name, const glm::vec4 &vec) const;
        void setMat4(const std::string &name, const glm::mat4 &mat) const;

    private:
        bool m_isCompiled;
        unsigned int m_ID;

};

#endif

