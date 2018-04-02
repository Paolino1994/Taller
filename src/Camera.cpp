#include "Camera.h"
#include <iostream>

Camera::Camera(World& world, int width, int height):
	world(world),
    width(width),
    height(height),
    x(0),
    y(0),
    x_update_offset(0),
    y_update_offset(0),
    followed(NULL)
{}

Camera::~Camera()
{}
    
void Camera::follow(Player* gameObj){ //Entity
    followed = gameObj;
}
    
void Camera::update(double dt){
    if (!followed) return;
    
    int old_x = this->x;
	int old_y = this->y;

    this->x = followed->getX() - this->width/2;
	// Si la posicion de camara esta pasando el limite del mundo en base a la pantalla
	if ((this->world.getWidth() - this->x) < this->width) {
		this->x = this->world.getWidth() - this->width;
	}
	else if (this->x < 0) { //< world.getWidthStart() pero deberiamos trabajar siempre con valores positivos (creo)
		this->x = 0;
	}
    this->x_update_offset = this->x - old_x;

	this->y = followed->getY() - this->height / 2;
	// Si la posicion de camara esta pasando el limite del mundo en base a la pantalla
	if ((this->world.getHeight() - this->y) < this->height) {
		this->y = this->world.getHeight() - this->height;
	}
	else if (this->y < 0) {
		this->y = 0;
	}
	this->y_update_offset = this->y - old_y;
}
    
void Camera::render(World& world){
	auto background = world.getBackground();
	auto entities = world.getEntities();

	// renderizamos el background (la cancha)
	background->setScaling(this->width, this->height);
	background->setSrcRect(this->x, this->y, this->width, this->height);
	background->render(0, 0);

	for (auto entity : entities)
	{
		int screen_x = entity->getX() - this->x;
		int screen_y = entity->getY() - this->y;
		//TODO: check screen_x/_y esten en mi ancho/alto
		//no dibujar lo que no veo!
		entity->render(screen_x, screen_y);
	}
}

