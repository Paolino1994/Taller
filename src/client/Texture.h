#pragma once
#include <memory>

#include "SDL.h"
#include "SDL_ttf.h"
#include "Surface.h"

class Texture
{
private:
    //Rendering context for this texture
    SDL_Renderer* mRenderer;
    //The actual hardware texture
    SDL_Texture* mTexture;
    SDL_Rect mSrcRect;
    SDL_Rect mDstRect;
    
    //Image dimensions
    int mWidth;
    int mHeight;
        
public:
    //Create texture from Surface
    Texture(SDL_Renderer* renderer, const Surface& surface);
    //Create texture from specified image file
    Texture(SDL_Renderer* renderer, std::string file);
    // Empty Texture
    Texture(SDL_Renderer *renderer);

    ~Texture();
    
    //Set color modulation
    void setColor( Uint8 red, Uint8 green, Uint8 blue );
    
    //Set blending
    void setBlendMode( SDL_BlendMode blending );

    //Set alpha modulation
    void setAlpha( Uint8 alpha );
        
    //Set destination scaling: width and height:
    void setScaling(int width, int height);
        
    //Set source rectangle data:
    void setSrcRect(int x, int y, int w, int h);
    
    //Get source rect data: (TEMP?)
    const SDL_Rect& getSrcRect();
    
    //Renders texture at given point
    void render(int x, int y); //only specify point
    void render(int x, int y, double angle, SDL_Point* center, SDL_RendererFlip flip);
    
    //Gets image dimensions
    int getWidth();
    int getHeight();
    
    int getScaledWidth() {return mDstRect.w;};
    int getScaledHeight() {return mDstRect.h;};

};

