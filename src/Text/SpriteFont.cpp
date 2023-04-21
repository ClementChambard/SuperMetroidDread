#include "SpriteFont.hpp"
#include <fstream>
#include <algorithm>
#include <TextureManager.h>
#include <Engine.hpp>
#include <DrawFuncs.h>

SpriteFont* SpriteFont::last_created_instance = nullptr;

SpriteFont::SpriteFont(std::string filename) {
    std::ifstream file(filename);

    std::string texture_file;
    file >> texture_file;
    texId = NSEngine::TextureManager::RegisterTexture(texture_file.c_str());

    while (!file.eof()) {
        SpriteFont::Glyph g;
        file >> g;
        glyphs.push_back(g);
    }
    glyphs[0].c = ' ';

    file.close();
    last_created_instance = this;
}

SpriteFont::~SpriteFont() {}

float calcTextSize(std::string text, std::vector<SpriteFont::Glyph> const& glyphs, float s)
{
    float size = 0.f;
    for (size_t i = 0; i < text.size(); i++) {
        char c = text[i];
        auto G = std::find_if(glyphs.begin(), glyphs.end(), [c](SpriteFont::Glyph g){ return g.c == c; });
        if (G == glyphs.end()) continue;
        if (i == text.size()-1) size += G->size.x * s;
        else size += G->x_advance * s;
    }
    return size;
}
float calcTextHeight(std::string text, std::vector<SpriteFont::Glyph> const& glyphs, float s)
{
    float maxTop = 0.f;
    float minBot = 0.f;
    for (size_t i = 0; i < text.size(); i++) {
        char c = text[i];
        auto G = std::find_if(glyphs.begin(), glyphs.end(), [c](SpriteFont::Glyph g){ return g.c == c; });
        if (G == glyphs.end()) continue;
        float ma = (G->size.y/2.f+G->offset.y)*s;
        float mi = (-G->size.y/2.f+G->offset.y)*s;
        if (maxTop < ma) maxTop = ma;
        if (minBot > mi) minBot = mi;
    }
    return maxTop - minBot;
}

void SpriteFont::renderText(std::string text, glm::vec2 pos, glm::vec2 charSize, glm::vec<4, uint8_t> col)
{
    float totalWidth = calcTextSize(text, glyphs, charSize.x);
    float totalHeight = calcTextHeight(text, glyphs, charSize.y);
    float firstGlyphXoff;
    std::vector<Glyph>::iterator G;
    if (text.size() > 0) if ((G = std::find_if(glyphs.begin(), glyphs.end(), [text](Glyph g){ return g.c == text[0]; })) != glyphs.end()) firstGlyphXoff = G->size.x/2.f*charSize.x;
    if (halign == LEFT) pos.x += firstGlyphXoff;
    else if (halign == CENTER) pos.x -= totalWidth / 2.f - firstGlyphXoff;
    else if (halign == RIGHT) pos.x -= totalWidth - firstGlyphXoff;
    if (valign == TOP) pos.y -= totalHeight / 2.f;
    else if (valign == BOTTOM) pos.y += totalHeight / 2.f;
    for (char c : text) {
        auto G = std::find_if(glyphs.begin(), glyphs.end(), [c](Glyph g){ return g.c == c; });
        if (G == glyphs.end()) continue;
        glm::vec2 p = pos+G->offset;
        glm::vec2 s = G->size*charSize;
        glm::vec4 tl = {p.x-s.x/2.f, p.y-s.y/2.f, G->uvs.x, G->uvs.w};
        glm::vec4 tr = {p.x+s.x/2.f, p.y-s.y/2.f, G->uvs.z, G->uvs.w};
        glm::vec4 br = {p.x+s.x/2.f, p.y+s.y/2.f, G->uvs.z, G->uvs.y};
        glm::vec4 bl = {p.x-s.x/2.f, p.y+s.y/2.f, G->uvs.x, G->uvs.y};
        NSEngine::draw_quad_tex_2d(texId, tl, tr, br, bl);
        // COLOR !
        pos.x += G->x_advance*charSize.x;
    }
}

std::istream& operator>>(std::istream& l, SpriteFont::Glyph& r) {
    return (l >> r.c >> r.size.x >> r.size.y >> r.offset.x >> r.offset.y >> r.uvs.x >> r.uvs.y >> r.uvs.z >> r.uvs.w >> r.x_advance);
}
