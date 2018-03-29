//
// Created by root on 29/03/18.
//

#ifndef UNTITLED1_JUGADOR_H
#define UNTITLED1_JUGADOR_H
#include <string>
#include <iostream>


class Jugador {
private:
    char* nombre;

public:
    void asignarNombre(const char* string);

    char *getNombre();

    void asignarNombre(char *string);
};


#endif //UNTITLED1_JUGADOR_H
