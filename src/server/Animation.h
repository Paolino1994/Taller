#pragma once

#include "common/GameConstants.h"
// Animaciones: tiras de sprites HORIZONTALES!, 
// width / frames = width de sprite individual
// height = height de sprite individual
// fps: velocidad de actualizacion de la animacion
// -------> cuantos frames cambia por segundo:
// frametime: cuanto tarda un frame hasta el proximo
// -------------------
// DEL LADO DEL SERVER:
// Implementa solamente la logica de la animacion -> cambios de frames y reseteos
class Animation
{
private:
    // Time info
    double time;
    int frames;
    // int fps; innecesario
    double frametime;
    //
    int presentFrame;
    int frameWidth;
public:
	Animation(int frames, int fps);
	Animation(const sprite_info_t& sprite_info);
    /*Animation(const Animation& other);
    Animation& operator=(const Animation& other);*/
    ~Animation();
    
    //Para animaciones que dependen de la fisica
    void setFrameTime(double new_frametime) {frametime = new_frametime;};
    //Reset the animation to the starting frame
    void reset();
    //Update the animation with the following change in time (deltatime)
    void update(double deltatime);
    
/*    //Renders corresponding animation texture at given point
    void render(int x, int y); //only specify point
	void render(int x, int y, double angle); //center va a ser centro de imagen actual, flip = NONE
    void render(int x, int y, double angle, SDL_Point* centre, SDL_RendererFlip flip);
 */   
  //  int getWidth();
  //  int getHeight();
    int getFrames();
    int getPresentFrame();
    
/*	// el ancho (posiblemente escalado) de lo que estariamos dibujando
    int getDestinationWidth() {return mTexture.getScaledWidth();};

	// el alto (posiblemente escalado) de lo que estariamos dibujando
    int getDestinationHeight() {return mTexture.getScaledHeight();};
	*/
};

