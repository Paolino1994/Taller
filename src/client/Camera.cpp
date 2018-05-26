#include "Camera.h"
#include "common/Log.h"
#include <iostream>


Camera::Camera(World& world, int width, int height, int widthScrollOffset, int heightScrollOffset, Texture *miniMap, Texture *miniField):
	world(world),
    width(width),
    height(height),
	widthScrollOffset(widthScrollOffset),
	heightScrollOffset(heightScrollOffset),
    x(0),
    y(0),
    x_update_offset(0),
    y_update_offset(0),
    followed(world.getBall()),
	miniMapRect(miniMap),
	miniFieldRect(miniField)
{
}

Camera::~Camera()
{}
    
void Camera::follow(Entity& gameObj){ //Entity
    followed = gameObj;
}

bool Camera::isWithinScrollBoundaries(Entity * entity)
{
	// quizas hay que manejar el caso en que la camara este en el limite del mundo y haya jugadores en el offset -> actualmente no podriamos cambiar a estos
	return (this->x + widthScrollOffset < entity->getCenterX()) && (entity->getCenterX() < this->x + this->width - widthScrollOffset) &&
		(this->y + heightScrollOffset < entity->getCenterY()) && (entity->getCenterY() < this->y + this->height - heightScrollOffset);
}
    
void Camera::update(double dt){
	int old_x = this->x;
	int old_y = this->y;

	// Actualizamos el x de la camara
	
	// if no esta en mis limites de x del scroll
	if (this->x + widthScrollOffset > followed.getCenterX()) { // limite de x a izquierda
		this->x = followed.getCenterX() - widthScrollOffset;
	}
	else if (this->x + this->width - widthScrollOffset < followed.getCenterX()) { //limite de x a derecha
		this->x = followed.getCenterX() - this->width + widthScrollOffset;
	}

	// Si la posicion de camara esta pasando el limite del mundo en base a la pantalla
	if ((this->world.getWidth() - this->x) < this->width) {
		this->x = this->world.getWidth() - this->width;
	}
	else if (this->x < 0) { //< world.getWidthStart() pero deberiamos trabajar siempre con valores positivos (creo)
		this->x = 0;
	}
    this->x_update_offset = this->x - old_x;

	// Actualizamos el y de la camara

	// if no esta en mis limites de Y del scroll
	if (this->y + heightScrollOffset > followed.getCenterY()) { // limite de y superior
		this->y = followed.getCenterY() - heightScrollOffset;
	}
	else if (this->y + this->height - heightScrollOffset < followed.getCenterY()) { //limite de y inferior
		this->y = followed.getCenterY() - this->height + heightScrollOffset;
	}

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
	auto players = world.getPlayers();
	// auto playerSelectedTexture = world.getPlayerSelectedTexture();

	// renderizamos el background (la cancha)
	background->setScaling(this->width, this->height);
	background->setSrcRect(this->x, this->y, this->width, this->height);
	background->render(0, 0);

	miniFieldRect->render(this->x / 10, this->y / 10);

	miniMapRect->render(miniFieldRect->getPosX() -10 + followed.getX() / 10, miniFieldRect->getPosY() -10 + followed.getY() / 10);


	for (std::pair<const Player_ID, Player>& pair : players)
	{
		Player& player = pair.second;
		int screen_x = player.getX() - this->x;
		int screen_y = player.getY() - this->y;
		//TODO: check screen_x/_y esten en mi ancho/alto
		//no dibujar lo que no veo!
		if(player.isControlledByMe()){
			// playerSelectedTexture->render(screen_x + 5, screen_y -10);
		}
		player.render(screen_x, screen_y);
		player.renderMiniMap(miniMapRect->getPosX(), miniMapRect->getPosY());
	}

	int screen_x = world.getBall().getX() - this->x;
	int screen_y = world.getBall().getY() - this->y;
	Log::get_instance()->info("X: " + std::to_string(screen_x) + " Y: " + std::to_string(screen_y));
	world.getBall().render(screen_x, screen_y);
	world.getBall().renderMiniMap(miniFieldRect->getPosX(), miniFieldRect->getPosY());
}

