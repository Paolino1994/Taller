//
// Created by federico on 06/04/18.
//
#define EQUIPO1 0
#define EQUIPO2 1
#define JUGADOR1 1
#define JUGADOR2 2
#define JUGADOR3 3
#define JUGADOR4 4
#define JUGADOR5 5
#define JUGADOR6 6
#ifndef UNTITLED_YAMLREADER_H
#define UNTITLED_YAMLREADER_H
#include <yaml.h>
#include <map>

class YAMLReader {

public:
    void setFile(char *string);

    const char *archivo;

    std::string getNombreEquipo(int i);


    void destroy();

    bool esNombre(char *stream);

    char *getNombre(char *stream);



    std::string getSpritesEquipo(int i);

    void printType(yaml_token_type_t e);



    void printAll();


    void printAll(int arch);

    std::string getFormacion(int equipo);

    void readAll(int arch);

    std::map<std::string, std::string> getEverything(int equipo);

private:

    yaml_token_t token;

    yaml_document_t document[4];

    yaml_parser_t parser[4];
    yaml_parser_t parser2;

    FILE *equipo1;

    FILE* files[4];

    FILE *startEquipo(char *equipo, int arch);

    std::string find(std::string string, int equipo);


    void destroyFile(int i);


    std::string findJugador(std::string basic_string, int equipo);

    std::string findnext(int equipo);
};


#endif //UNTITLED_YAMLREADER_H
