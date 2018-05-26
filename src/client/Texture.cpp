#include <exception>

#include "Texture.h"
#include "SDL.h"

Texture::Texture(SDL_Renderer* renderer, const Surface& surface){
    mTexture = SDL_CreateTextureFromSurface(renderer, surface.get());
    if (!mTexture){
        printf( "Unable to create texture from Surface, SDL Error: %s\n", SDL_GetError() );
        throw std::runtime_error("AAA");
    }
    
    mWidth = surface.getWidth();
    mHeight = surface.getHeight();
    
    mSrcRect = mDstRect = {0, 0, mWidth, mHeight};
    mRenderer = renderer;
}

Texture::Texture(SDL_Renderer *renderer):
    mRenderer(renderer){

    }


Texture::Texture(SDL_Renderer* renderer, std::string file):
    Texture(renderer, Surface(file))
{}

Texture::~Texture()
{
    SDL_DestroyTexture(mTexture);
    mTexture = nullptr;
    mRenderer = nullptr;
}

void Texture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
    //Modulate texture rgb
    SDL_SetTextureColorMod(mTexture, red, green, blue); //check return
}

void Texture::setBlendMode(SDL_BlendMode blending)
{
    //Set blending function
    SDL_SetTextureBlendMode(mTexture, blending); //check return
}
        
void Texture::setAlpha(Uint8 alpha)
{
    //Modulate texture alpha
    SDL_SetTextureAlphaMod(mTexture, alpha); //check return 
}

//Set destination scaling: width and height:
void Texture::setScaling(int width, int height){
    mDstRect.w = width;
    mDstRect.h = height;
}
        
//Set source rectangle data:
void Texture::setSrcRect(int x, int y, int w, int h){
    mSrcRect = {x, y, w, h};
}

const SDL_Rect& Texture::getSrcRect(){
    return mSrcRect;
}

//Renders texture at given point, only specify point
void Texture::render(int x, int y){
    mDstRect.x = x;
    mDstRect.y = y;
    SDL_RenderCopy(mRenderer, mTexture, &mSrcRect, &mDstRect); //check return
}

//Renders texture at given point
void Texture::render(int x, int y, double angle, SDL_Point* center, SDL_RendererFlip flip){
    mDstRect.x = x;
    mDstRect.y = y;
    SDL_RenderCopyEx(mRenderer, mTexture, &mSrcRect, &mDstRect, angle, center, flip); //check return
}

int Texture::getWidth()
{
    return mWidth;
}

int Texture::getHeight()
{
    return mHeight;
}

int Texture::getPosX()
{
    return mDstRect.x;
}

int Texture::getPosY()
{
    return mDstRect.y;
}
