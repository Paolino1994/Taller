//
// Created by root on 28/03/18.
//

// SDL2 Hello, World!
// This should display a white screen for 2 seconds
// compile with: clang++ main.cpp -o hello_sdl2 -lSDL2
// run with: ./hello_sdl2
#include <SDL2/SDL.h>
#include <stdio.h>
#include "Partido.h"
#include "iostream"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

int main(int argc, char* args[]) {
    std::cout << "Boca vs River" << std::endl;
    Partido partido("Boca","River");
    partido.iniciarRandom();
    partido.mostrarEquipos();
    return 0;
}