#pragma once
#include <stdexcept>

#include "Event.h"

class UnhandledEventException : public std::logic_error
{
public:
	UnhandledEventException() : std::logic_error("Event not handled by this Handler!") { };
};

class EventHandler
{
public:
	EventHandler();
	virtual ~EventHandler();

	void registerTo(EventID eventId);

	// Por si algun Handler prefiere definir un solo metodo para todo los eventos especificos
	// Esto va a tener sentido para serializar los eventos y mandarlos por red
	virtual void handleFallback(Event& e) {
		throw UnhandledEventException();
	}

	// Aca para abajo ingresar todos los handles por cada tipo de evento ESPECIFICO!
	// Los que hereden de EventHandler solo tienen que redefinir los eventos que van a manejar! (sino lanzamos except)

	virtual void handle(KickEvent& e) { this->handleFallback(e); };
};

