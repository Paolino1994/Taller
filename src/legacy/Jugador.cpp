//
// Created by root on 29/03/18.
//

#include "Jugador.h"

void Jugador::asignarNombre(const char *string) {
    nombre= const_cast<char *>(string);
}

char *Jugador::getNombre() {
    return nombre;
}
