#pragma once

#include "SDL.h"
#include "SDL_ttf.h"
#include <string>
#include <iostream>
#include <sstream>


class Texto
{
private:
    SDL_Texture *text_texture = nullptr;
    mutable SDL_Rect text_rect;
    SDL_Renderer *mRenderer;
    TTF_Font * mFont;
    std::string text;
    
public:
    Texto(SDL_Renderer *renderer, const std::string &font_path, int font_size, const std::string &mensaje_texto, const SDL_Color &color);
    ~Texto();

    void display(int x, int y) const;

    SDL_Texture *loadFont(const std::string &font_path, int font_size, const std::string &mensaje_texto, const SDL_Color &color);

    void getTextureDimensions(int *w, int *h);

    bool updateText( std::string textureText, SDL_Color textColor );

    void free();


};

