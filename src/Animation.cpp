#include <iostream>

#include "Animation.h"

Animation::Animation(Texture& texture, int frames, int fps):
    mTexture(texture),
    time(0),
    frames(frames),
    frametime(1.0 / fps),
    presentFrame(0),
    frameWidth(texture.getWidth() / frames) //assert(width % frames == 0)
{
}

Animation::Animation(Texture& texture, const sprite_info_t& sprite_info):
    mTexture(texture),
    time(0),
    frames(sprite_info.frames),
    frametime(1.0 / sprite_info.frames_per_second),
    presentFrame(0),
    frameWidth(texture.getWidth() / frames) //assert(width % frames == 0)
{    
	mTexture.setScaling(sprite_info.width, sprite_info.height);
}

Animation::~Animation()
{}
    
//Reset the animation to the starting frame
void Animation::reset(){
    presentFrame = 0; //innecesario
    time = 0;
}

//Update the animation with the following change in time (deltatime)
void Animation::update(double deltatime){
    this->time += deltatime;
    this->presentFrame = static_cast<int>(time / frametime) % frames;
    //std::cout << "Time es " << time << " y el present frame es " << presentFrame << std::endl;
}
    
//Renders corresponding animation texture at given point and time change
void Animation::render(int x, int y){ //only specify point
    mTexture.setSrcRect(presentFrame * frameWidth, 0, frameWidth, mTexture.getHeight());
    mTexture.render(x,y);
}

void Animation::render(int x, int y, double angle, SDL_Point* center, SDL_RendererFlip flip){
    mTexture.setSrcRect(presentFrame * frameWidth, 0, frameWidth, mTexture.getHeight());
    mTexture.render(x,y, angle, center, flip);
}

int Animation::getWidth()
{
    return frameWidth;
}

int Animation::getHeight()
{
    return mTexture.getHeight();
}

int Animation::getFrames()
{
    return frames;
}

int Animation::getPresentFrame()
{
    return presentFrame;
}