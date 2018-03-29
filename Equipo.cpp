//
// Created by root on 29/03/18.
//

#include "Equipo.h"

Equipo::Equipo(const char *string) {
    nombre= const_cast<char *>(string);
}

void Equipo::generarRandom() {
    Jugador* players=jugadores.get_allocator().allocate(6);
    players[0].asignarNombre("Pedro Gomez");
    players[1].asignarNombre("Juan Perez");
    players[2].asignarNombre("Anibal Gonzales");
    players[3].asignarNombre("Jorge Rodriguez");
    players[4].asignarNombre("Jose Garcia");
    players[5].asignarNombre("Carlos Gutierrez");
}


std::list<Jugador> Equipo::getJugadores() {
    return jugadores;
}

Equipo::Equipo() {}


void Equipo::setNombre(const char *string) {
    nombre= const_cast<char *>(string);
}
char* Equipo::getNombre() {
    return nombre;
}



