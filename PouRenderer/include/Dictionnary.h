#ifndef DICTIONNARY_H
#define DICTIONNARY_H

#include <string>
#include <unordered_map>

class Dictionnary
{
    public:
        Dictionnary();
        virtual ~Dictionnary();

        bool loadFromFile(const std::string &filepath);

        std::string lookForWord(const std::string &word);

    protected:

    private:
        std::unordered_map<std::string, std::string> m_wordsMap;
        std::unordered_map<std::string, std::string> m_alreadyLookedWordsMap;
};

#endif // DICTIONNARY_H
