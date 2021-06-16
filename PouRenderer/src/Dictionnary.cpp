#include "Dictionnary.h"

#include <iostream>
#include <vector>

#include "Utils.h"

Dictionnary::Dictionnary()
{
    //ctor
}

Dictionnary::~Dictionnary()
{
    //dtor
}

bool Dictionnary::loadFromFile(const std::string &filepath)
{
    std::ifstream dataFile(filepath);

    for( std::string line; getline( dataFile, line ); )
    {
        std::string left, right;
        auto equalPos = line.find_first_of('=');

        if(equalPos == std::string::npos)
        {
            left = formatString(removeWhite(line));
            right = removeWhite(line);
        } else {
            left = formatString(removeWhite(line.substr(0,equalPos)));
            right = removeWhite(line.substr(equalPos+1));
        }


        m_wordsMap.insert({left, right});
        m_alreadyLookedWordsMap.insert({left, right});
    }

    return (true);
}

int computeLevenshteinDistance(const std::string word1, const std::string word2)
{
    int m = word1.length();
    int n = word2.length();

    std::vector<int> d((m+1)*(n+1),0); //(m+1) lines, (n+1) columns

    for(int i = 1 ; i <= m ; ++i)
        d[i*(n+1)] = i;

    for(int j = 1 ; j <= n ; ++j)
        d[j] = j;

    int cost = 0;

    for(int j = 1 ; j <= n ; ++j)
    {
        for(int i = 1 ; i <= m ; ++i)
        {
            cost = (word1[i-1] == word2[j-1]) ? 0 : 1;
            d[i*(n+1) + j] = std::min(std::min(
                                        d[(i-1)*(n+1)+j]+1,
                                        d[i*(n+1)+(j-1)]+1),
                                        d[(i-1)*(n+1)+(j-1)]+cost);
        }
    }

    return d[m*(n+1)+n];
}

std::string Dictionnary::lookForWord(const std::string &word)
{
    auto nWord = formatString(removeWhite(word));

    auto founded = m_alreadyLookedWordsMap.find(nWord);
    if(founded != m_alreadyLookedWordsMap.end())
        return founded->second;

    /*founded =  m_wordsMap.find(nWord+"s");
    if(founded != m_wordsMap.end())
        return founded->second;

    if(nWord.back() == 's')
    {
        founded =  m_wordsMap.find(nWord.substr(0,nWord.size()-1));
        if(founded != m_wordsMap.end())
            return founded->second;
    }*/

    std::string bestMatch, bestMatchSrc;
    int bestMatchValue = -1;

    for(auto dicoWord : m_wordsMap)
    {
        int d = computeLevenshteinDistance(dicoWord.first, nWord);

        if(bestMatchValue == -1 || d < bestMatchValue)
        {
            bestMatchValue = d;
            bestMatchSrc = dicoWord.first;
            bestMatch = dicoWord.second;
        }

        if(d == 1)
            break;
    }

    if(bestMatchValue <= 2 && bestMatchValue <= std::min(nWord.size()/3, word.size()/3))
    {
        std::cout<<"WARNING: Using dictionnary match with "<<bestMatchValue<<" errors \""<<nWord<<"\" matched with: \""<<bestMatchSrc<<"\""<<std::endl;

        m_alreadyLookedWordsMap.insert({nWord,bestMatch});

        return bestMatch;
    }

    /* compute Levenshtein distance using Wagner–Fischer algorithm, use best match if distance <= 2 ? */

    std::cout<<"New word: \""<<word<<"\""<<std::endl;
    m_wordsMap.insert({nWord,word});
    m_alreadyLookedWordsMap.insert({nWord,word});

    return word;
}


