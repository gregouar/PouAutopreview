#ifndef TEXTRENDERER_H
#define TEXTRENDERER_H


#include <map>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "Shader.h"


/// Holds all state information relevant to a character as loaded using FreeType
struct Character {
    unsigned int TextureID; // ID handle of the glyph texture
    glm::ivec2   Size;      // Size of glyph
    glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
    unsigned int Advance;   // Horizontal offset to advance to next glyph
};

class TextRenderer
{
    public:
        TextRenderer();
        virtual ~TextRenderer();

        bool initRendering(int windowWidth, int windowHeight, const std::string &fontName);
        void renderText(const std::string &text, float x, float y, float scale, glm::vec3 color);

    protected:
        void compileShader();

    private:
        int m_windowWidth;
        int m_windowHeight;

        Shader m_textShader;

        std::map<GLchar, Character> m_characters;
        unsigned int m_VAO, m_VBO;
};

#endif // TEXTRENDERER_H
