#pragma once

// Abstract class
// Object with a real world x and y position
// With width and height
// It may update its position in time
// Practicamente un SDL_Rect
class Entity
{
protected:
    double x; //real world x
    double y; //real world y
public:
	Entity() : x(0), y(0) {};
    Entity(double x, double y): x(x), y(y) {};
    virtual ~Entity(){};
    
    // update position
    // void update(double dt);
    
    int getX() const {return (int)x;}; //redondear?
    int getY() const {return (int)y;}; //redondear?

	int getCenterX() { return getX() + (getWidth() / 2); };
	int getCenterY() { return getY() + (getHeight() / 2); };

    
    virtual int getWidth() = 0;
    virtual int getHeight() = 0;
	virtual double getAngle() = 0;

    // render: screen_x and screen_y
    //virtual void render(int screen_x, int screen_y) = 0;
    
};

