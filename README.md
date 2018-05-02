# Tehkan Fiuba Cup

Juego para la materia Taller de programacion I
Primer cuatrimestre de 2018

Grupo: NEXUS

Integrantes:

- Ansaldo Francisco
- Guerini Francisco
- Mantilla Roger
- Murck Pablo
- Paolino Federico

Bibliotecas para compilar el proyecto:

- libyaml
- SDL2
- SDL_image
- SDL_mixer
- SDL_ttf

# Controles:

- Movimientos			-->			Flechas
- Cambiar Jugador		-->			Q
- Barrer				-->			Barra Espaciadora
- Correr				-->			R
- Patear				-->			Enter
- Salir del juego				-->			ESC

# Prueba Sockets
- Server    --> ./client.out server 8080
- Client    --> ./client.out client 127.0.0.1 8080
- #OBS
      Al terminar la pruba se verá un "Error: Transport endpoint is not connected" en el cliente. Esto no es un problema, simplemente al hacer el shutdown, intenta notificar al server de su cierre, pero este ya esta cerrado.
