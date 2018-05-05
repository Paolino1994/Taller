#pragma once

#include <map>
#include <vector>

#include "Entity.h"
#include "GameConstants.h"
#include "Animation.h"
#include "SDL.h"

class Player: public Entity
{
private:
    std::vector<Animation> mAnimations;
    double velX;
    double velY;
    PlayerState state;
    //SDL_RendererFlip flipmode; ahora se usa el angulo!
	double angle;
    const int MAX_VEL_X;
    const int MAX_VEL_Y;
public:
    Player(std::map<const std::string, Animation>& animationMapper, const player_data_t player_data, double initial_x, double initial_y);
    virtual ~Player();
    
    void handleEvent(SDL_Event& e);

    // update position
    // x: de 0 a x_limit
	// y: de 0 a y_limit 
	// misma logica que renderizacion en pantalla
    void update(double dt, int x_limit, int y_limit); // y_limit == piso

    virtual int getWidth();
    virtual int getHeight();

    // render: screen_x and screen_y
    virtual void render(int screen_x, int screen_y);
};

    
    