#include "Shader.h"

#include <iostream>
#include <fstream>

Shader::Shader() : m_isCompiled(false)
{

}

Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath) : Shader()
{
    this->loadFromFile(vertexPath, fragmentPath);
}

bool Shader::loadFromFile(const std::string &vertexPath, const std::string &fragmentPath)
{
     // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode   = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch(std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        return (false);
    }

    return this->compile(vertexCode, fragmentCode);
}

bool Shader::compile(const std::string &vertexCode, const std::string &fragmentCode)
{
    m_isCompiled = true;

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    // vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    // print compile errors if any
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        m_isCompiled = false;
    };

    // similar for Fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    // print compile errors if any
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        m_isCompiled = false;
    };

    // shader Program
    m_ID = glCreateProgram();
    glAttachShader(m_ID, vertex);
    glAttachShader(m_ID, fragment);
    glLinkProgram(m_ID);
    // print linking errors if any
    glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(m_ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        m_isCompiled = false;
    }

    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return (m_isCompiled);
}

void Shader::use()
{
    if(m_isCompiled)
        glUseProgram(m_ID);
}

void Shader::setBool(const std::string &name, bool value) const
{
    if(m_isCompiled)
        glUniform1i(glGetUniformLocation(m_ID, name.c_str()), (int)value);
}
void Shader::setInt(const std::string &name, int value) const
{
    if(m_isCompiled)
        glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
}
void Shader::setFloat(const std::string &name, float value) const
{
    if(m_isCompiled)
        glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
}

void Shader::setVec2(const std::string &name, const glm::vec2 &vec) const
{
    if(m_isCompiled)
        glUniform2fv(glGetUniformLocation(m_ID, name.c_str()), 1, &vec[0]);
}

void Shader::setVec3(const std::string &name, const glm::vec3 &vec) const
{
    if(m_isCompiled)
        glUniform3fv(glGetUniformLocation(m_ID, name.c_str()), 1, &vec[0]);
}

void Shader::setVec4(const std::string &name, const glm::vec4 &vec) const
{
    if(m_isCompiled)
        glUniform4fv(glGetUniformLocation(m_ID, name.c_str()), 1, &vec[0]);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
{
    if(m_isCompiled)
        glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}


