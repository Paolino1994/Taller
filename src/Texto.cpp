#include <iostream>

#include "Texto.h"


Texto::Texto(SDL_Renderer *renderer, const std::string &font_path, int font_size, const std::string &mensaje_texto, const SDL_Color &color){
   	mRenderer = renderer;
   	text_texture = loadFont(font_path, font_size, mensaje_texto, color);  		
   	SDL_QueryTexture(text_texture, nullptr, nullptr,&text_rect.w, &text_rect.h);
}

void Texto::display(int x, int y) const {
	text_rect.x = x;
	text_rect.y = y;
	SDL_RenderCopy(mRenderer, text_texture, nullptr, &text_rect);
}

SDL_Texture *Texto::loadFont(const std::string &font_path, int font_size, const std::string &mensaje_texto, const SDL_Color &color){
	TTF_Font *font = TTF_OpenFont(font_path.c_str(), font_size);
	if(!font){
		std::cerr << "failed to load font";
	}
	auto text_surface = TTF_RenderText_Solid(font, mensaje_texto.c_str(), color);
	if(!text_surface){
		std::cerr << "failed to create text surface";
	}
	auto text_texture = SDL_CreateTextureFromSurface(mRenderer, text_surface);
	if(!text_texture){
		printf("%s\n", SDL_GetError());
	}
	SDL_FreeSurface(text_surface);
	return text_texture;
}

void Texto::getTextureDimensions(int *w, int *h) {
	SDL_QueryTexture(text_texture, NULL, NULL, w, h);
}
