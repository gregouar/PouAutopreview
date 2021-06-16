#ifndef UTILS_H
#define UTILS_H

#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <fstream>
#include <string>

glm::vec3 readVec3(std::ifstream &dataFile);
glm::vec4 readVec4(std::ifstream &dataFile);

void replaceStr(std::string& str, const std::string& from, const std::string& to);
std::string removeWhite(const std::string &str);
std::string formatString(const std::string &str);

bool readBool(std::ifstream &dataFile);


#endif // UTILS_H
