#pragma once

#include <memory>

#include "SDL.h"
#include "common/Log.h"

class Surface
{
    private:
        SDL_Surface* mSurface;
        
public:
        Surface(std::string file);
        ~Surface();
        
        //get the underlying SDL_Surface;
        SDL_Surface* get() const;
        
        // Uint8: specify in hex
        void setColorKey(Uint8 red, Uint8 green, Uint8 blue);
		void disableColorKey();
        
        //Gets image dimensions
        int getWidth() const;
        int getHeight() const;
};

