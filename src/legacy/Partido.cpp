//
// Created by root on 29/03/18.
//

#include "Partido.h"
#include <iostream>
#include <list>

Partido::Partido(const char *equipoA, const char *equipoB) {
    equipo1.setNombre(equipoA);
    equipo2.setNombre(equipoB);

}

void Partido::iniciarRandom() {
    equipo1.generarRandom();
    equipo2.generarRandom();

}

void Partido::mostrarEquipos() {
    //std::cout << equipo1.getNombre() << std::endl;
    std::list<Jugador> jugadoresEquipo1=equipo1.getJugadores();
    for(int i=0;i<=5;i++){
        ////std::cout << *jugadoresEquipo1[i].getNombre() << std::endl;
    }
    //std::cout << equipo2.getNombre() << std::endl;
    std::list<Jugador> jugadoresEquipo2=equipo1.getJugadores();
    for(int i=0;i<=5;i++){
        ////std::cout << *jugadoresEquipo2[i].getNombre() << std::endl;
    }


}
