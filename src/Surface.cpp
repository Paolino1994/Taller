#include <exception>

#include "Surface.h"

#include "SDL_image.h"

Surface::Surface(std::string file)
{
    //Load image at specified path
    mSurface = IMG_Load(file.c_str());
    
    if(!mSurface){ //hacer exception!
        printf( "Unable to load image %s! SDL_image Error: %s\n", file.c_str(), IMG_GetError() );
        throw std::runtime_error("AAA");
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