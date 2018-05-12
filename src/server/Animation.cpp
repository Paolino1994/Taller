#include <iostream>

#include "Animation.h"

Animation::Animation(int frames, int fps):
    time(0),
    frames(frames),
    frametime(1.0 / fps),
    presentFrame(0)
{
}

Animation::Animation(const sprite_info_t& sprite_info):
    time(0),
    frames(sprite_info.frames),
    frametime(1.0 / sprite_info.frames_per_second),
    presentFrame(0)
{    
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

int Animation::getFrames()
{
    return frames;
}

int Animation::getPresentFrame()
{
    return presentFrame;
}