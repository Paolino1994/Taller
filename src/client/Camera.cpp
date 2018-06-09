#include "Camera.h"

#include "common/Log.h"

Camera::Camera(World& world, int width, int height, int widthScrollOffset, int heightScrollOffset):
	world(world),
    width(width),
    height(height),
	widthScrollOffset(widthScrollOffset),
	heightScrollOffset(heightScrollOffset),
    x(0),
    y(0),
    x_update_offset(0),
    y_update_offset(0),
    followed(world.getBall())
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
    
void Camera::render(World& world, int screen_x, int screen_y){
	auto background = world.getBackground();
	auto players = world.getPlayers();

	// renderizamos el background (la cancha)
	background->setScaling(this->width, this->height);
	background->setSrcRect(this->x, this->y, this->width, this->height);
	background->render(screen_x, screen_y);

	for (std::pair<const Player_ID, Player>& pair : players)
	{
		Player& player = pair.second;
		player.render(screen_x + player.getX() - this->x, screen_y + player.getY() - this->y);
	}

	Ball& ball = world.getBall();
	ball.render(screen_x + ball.getX() - this->x, screen_y + ball.getY() - this->y);
}

int Camera::getX()
{
	return this->x;
}

int Camera::getY()
{
	return this->y;
}

int Camera::getWidth()
{
	return this->width;
}

int Camera::getHeight()
{
	return this->height;
}

