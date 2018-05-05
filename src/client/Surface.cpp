#include <exception>

#include "Surface.h"

#include "SDL_image.h"

Surface::Surface(std::string file)
{
    mSurface = IMG_Load(file.c_str());    
    if(!mSurface){ 
        Log* log = Log::get_instance();
	    log->error("No se encontro la imagen: " + file);
        mSurface = IMG_Load("res/image_not_found.png");
    }
}

Surface::~Surface()
{
    SDL_FreeSurface(mSurface);
}

void Surface::setColorKey(Uint8 red, Uint8 green, Uint8 blue){
    SDL_SetColorKey(mSurface, 1, SDL_MapRGB(mSurface->format, red, green, blue));
    //check return value!
}

//get the underlying SDL_Surface,
SDL_Surface* Surface::get() const{
    return mSurface;
}

void Surface::disableColorKey(){
	SDL_SetColorKey(this->mSurface, 0, 0xFFFFFF);
}

int Surface::getWidth() const{
    return mSurface->w;
}

int Surface::getHeight() const{
    return mSurface->h;
}