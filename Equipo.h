//
// Created by root on 29/03/18.
//

#ifndef UNTITLED1_EQUIPO_H
#define UNTITLED1_EQUIPO_H


#include "Jugador.h"
#include <iostream>
#include <list>

class Equipo {
private:
    std::list<Jugador> jugadores;
    char* nombre;

public:
    void generarRandom();
    std::list<Jugador> getJugadores();
    Equipo(const char *string);

    Equipo(Equipo *pEquipo);
    Equipo();
    void setNombre(const char *string);

    char * getNombre();
};


#endif //UNTITLED1_EQUIPO_H
