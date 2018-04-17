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
#include <vector>

class YAMLReader {

public:
    static YAMLReader *get_instance();

    std::map<std::string, std::string> getEverything(int equipo);

    void destroy();

    std::string getNombre(int equipo);

    std::string getSpriteStill(int equipo);

    std::string getSpriteRunning(int equipo);

    std::string getNombreJugador(int equipo, int jugador);

    std::string getSpriteSweeping(int equipo);

    std::string getSpriteKicking(int equipo);


    int getDefensores(int i);

    int getMediocampistas(int equipo);

    int getDelanteros(int equipo);

private:

    YAMLReader(std::string string);

    std::map<std::string,std::string> infoEquipo[2];

    yaml_token_t token;

    yaml_document_t document[4];

    yaml_parser_t parser[4];

    yaml_parser_t parser2;

    FILE* files[4];

    FILE *startEquipo(std::string equipo, int arch);

    std::string find(std::string string, int equipo);

    void destroyFile(int i);

    std::string findJugador(std::string basic_string, int equipo);

    std::string findnext(int equipo);


    std::string getSpritesEquipo(int i);

    void printType(yaml_token_type_t e);


    void printAll(int arch);

    std::string getFormacion(int equipo);

    void readAll(int arch);

    void readArchives();

    static YAMLReader *instance;

    std::map<std::string, std::string> cargarDefault();

    bool mapIsValid(std::map<std::string, std::string> map, std::vector<std::string> vector);


};


#endif //UNTITLED_YAMLREADER_H
