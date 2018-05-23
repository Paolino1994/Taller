#pragma once

#include "common/GameConstants.h"
#include "Texture.h"
// Animaciones: tiras de sprites HORIZONTALES!, 
// width / frames = width de sprite individual
// height = height de sprite individual
// fps: velocidad de actualizacion de la animacion
// -------> cuantos frames cambia por segundo:
// frametime: cuanto tarda un frame hasta el proximo
class Animation
{
private:
    // Texture with spritesheet
    Texture& mTexture;
    // Time info
    double time;
    int frames;
    // int fps; innecesario
    double frametime;
    //
    int presentFrame;
    int frameWidth;
	SDL_Point rotationCentre;
public:
    Animation(Texture& texture, int frames, int fps);
    Animation(Texture& texture, const sprite_info_t& sprite_info);
    /*Animation(const Animation& other);
    Animation& operator=(const Animation& other);*/
    ~Animation();
    
    //Para animaciones que dependen de la fisica
    void setFrameTime(double new_frametime) {frametime = new_frametime;};
    //Reset the animation to the starting frame
    void reset();
    //Update the animation with the following change in time (deltatime)
    void update(double deltatime);
    
    //Renders corresponding animation texture at given point
    void render(int x, int y); //only specify point
	void render(int x, int y, double angle); //center va a ser centro de imagen actual, flip = NONE
    void render(int x, int y, double angle, SDL_Point* centre, SDL_RendererFlip flip);
    
    int getWidth();
    int getHeight();
    int getFrames();
    int getPresentFrame();
	void setPresentFrame(size_t frame);
    
	// el ancho (posiblemente escalado) de lo que estariamos dibujando
    int getDestinationWidth() {return mTexture.getScaledWidth();};

	// el alto (posiblemente escalado) de lo que estariamos dibujando
    int getDestinationHeight() {return mTexture.getScaledHeight();};

    void render(int x, int y, double angle, double zoom);
};

