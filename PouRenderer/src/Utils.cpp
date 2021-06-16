#include "Utils.h"

#include <algorithm>

glm::vec3 readVec3(std::ifstream &dataFile)
{
    glm::vec3 v;

    std::string fieldStr;
    dataFile>>fieldStr;
    v.x = std::stof(fieldStr);
    dataFile>>fieldStr;
    v.y = std::stof(fieldStr);
    dataFile>>fieldStr;
    v.z = std::stof(fieldStr);

    return v;
}

glm::vec4 readVec4(std::ifstream &dataFile)
{
    glm::vec4 v;

    std::string fieldStr;
    dataFile>>fieldStr;
    v.x = std::stof(fieldStr);
    dataFile>>fieldStr;
    v.y = std::stof(fieldStr);
    dataFile>>fieldStr;
    v.z = std::stof(fieldStr);
    dataFile>>fieldStr;
    v.w = std::stof(fieldStr);

    return v;
}


std::string removeWhite(const std::string &str)
{
    auto copyStr = str;
    //copyStr.erase(copyStr.begin(), std::find_if(copyStr.begin(), copyStr.end(), std::bind1st(std::not_equal_to<char>(), ' ')));
    while(!copyStr.empty() && copyStr.front() == ' ')
        copyStr.erase(copyStr.begin());
    while(!copyStr.empty() && copyStr.back() == ' ')
        copyStr.pop_back();

    return copyStr;
}



void replaceStr(std::string& str, const std::string& from, const std::string& to)
{
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return;
    str.replace(start_pos, from.length(), to);
}

std::string formatString(const std::string &str)
{
    std::string rstr(str);

    std::transform (rstr.begin(), rstr.end(), rstr.begin(), ::tolower);

    replaceStr(rstr, " ", "_");
    replaceStr(rstr, "é", "e");
    replaceStr(rstr, "Ã©", "e");
    replaceStr(rstr, "è", "e");
    replaceStr(rstr, "Ã¨", "e");
    replaceStr(rstr, "à", "a");
    replaceStr(rstr, "Ã", "a");

    /*std::replace(rstr.begin(), rstr.end(), ' ', '_');
    std::replace(rstr.begin(), rstr.end(), 'é', 'e');
    std::replace(rstr.begin(), rstr.end(), 'è', 'e');
    std::replace(rstr.begin(), rstr.end(), 'à', 'e');*/


    return rstr;
}

bool readBool(std::ifstream &dataFile)
{
    std::string str;
    dataFile>>str;
    str = formatString(removeWhite(str));

    if(str == "true" || str == "1")
        return (true);

    return (false);
}
