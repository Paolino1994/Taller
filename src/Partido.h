//
// Created by root on 29/03/18.
//

#ifndef UNTITLED1_PARTIDO_H
#define UNTITLED1_PARTIDO_H


#include "Equipo.h"
#include "Resultado.h"
#include <iostream>
#include <list>

class Partido {
    Equipo equipo1, equipo2;
    Resultado resultado;
    Jugador* amonestados;
    std::list<Jugador> expulsados;
public:
    Partido(const char* string, const char* string1);
    void iniciarRandom();

    void mostrarEquipos();
};


#endif //UNTITLED1_PARTIDO_H
