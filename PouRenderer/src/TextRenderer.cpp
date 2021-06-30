#include "TextRenderer.h"


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

TextRenderer::TextRenderer()
{
    //ctor
}

TextRenderer::~TextRenderer()
{
    //dtor
}


bool TextRenderer::initRendering(int windowWidth, int windowHeight, const std::string &fontName)
{
    m_windowWidth = windowWidth;
    m_windowHeight = windowHeight;

    this->compileShader();


    // FreeType
    // --------
    FT_Library ft;
    // All functions return a value different than 0 whenever an error occurred
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return (false);
    }

	// load font as face
    FT_Face face;
    if (FT_New_Face(ft, fontName.c_str(), 0, &face)) {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        return (false);
    }
    else {
        // set size to load glyphs as
        FT_Set_Pixel_Sizes(face, 0, (int)(32*windowHeight/800.0f));

        // disable byte-alignment restriction
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        // load first 128 characters of ASCII set
        for (unsigned char c = 0; c < 128; c++)
        {
            // Load character glyph
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                continue;
            }
            // generate texture
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
            );
            // set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // now store character for later use
            Character character = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                static_cast<unsigned int>(face->glyph->advance.x)
            };
            m_characters.insert(std::pair<char, Character>(c, character));
        }
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    // destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);


    // configure VAO/VBO for texture quads
    // -----------------------------------
    /**glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);**/

     float quadVertices[] = {
        // positions        // texture Coords
         0.0f,  1.0f, 0.0f, 0.0f,
         0.0f,  0.0f, 0.0f, 1.0f,
         1.0f,  1.0f, 1.0f, 0.0f,
         1.0f,  0.0f, 1.0f, 1.0f,
    };
    // setup plane VAO
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    return (true);
}


void TextRenderer::renderText(const std::string &text, float x, float y, float scale, glm::vec3 color)
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    // activate corresponding render state
    m_textShader.use();
    m_textShader.setVec3("textColor",color);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(m_VAO);

    // iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = m_characters[*c];

        float xpos = x + ch.Bearing.x/m_windowWidth * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y)/m_windowHeight * scale;

        float w = ch.Size.x * scale /m_windowWidth;
        float h = ch.Size.y * scale /m_windowHeight;
        // update VBO for each character
        /**float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };**/
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // update content of VBO memory
        /**glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);**/


        m_textShader.setVec2("position",glm::vec2(xpos, ypos));
        m_textShader.setVec2("extent",glm::vec2(w,h));
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);


        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale / m_windowWidth; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);


    glDisable(GL_BLEND);
}


////////////////////////////////////////////////:


void TextRenderer::compileShader()
{
    std::string vertCode =
"#version 430 core\n"
"layout (location = 0) in vec2 aPos; "
"layout (location = 1) in vec2 aTex; "
"out vec2 TexCoords;"
""
"uniform mat4 projection;"
"uniform vec2 position;"
"uniform vec2 extent;"
""
"void main()"
"{"
//"    gl_Position = projection * vec4(aPos.xy, 0.0, 1.0);"
"   gl_Position.xy = position+extent*(aPos.xy);"
"   gl_Position.zw = vec2(0.0,1.0);"
//"   gl_Position = projection * gl_Position;"
"    TexCoords = aTex.xy;"
"}  ";

    std::string fragCode =
"#version 430 core\n"
"in vec2 TexCoords;"
"out vec4 color;"
""
"uniform sampler2D text;"
"uniform vec3 textColor;"
""
"void main()"
"{    "
"    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);"
"    color = vec4(textColor, 1.0) * sampled;"
"}  ";

    m_textShader.compile(vertCode, fragCode);

//    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(m_windowWidth), 0.0f, static_cast<float>(m_windowHeight));
    m_textShader.use();
 //   m_textShader.setMat4("projection", projection);
//    glUniformMatrix4fv(glGetUniformLocation(m_textShader.m_ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
}

