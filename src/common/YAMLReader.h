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
#include <yaml-cpp/yaml.h>
#include <map>
#include <vector>

class YAMLReader {

public:
    
    YAMLReader();

    void readYamlGeneral(std::string string);  
    
    void readYamlEquipos();  

    void destroy();

    std::string getTeamNombre(int equipo);

    std::string getTeamColor(int equipo);

    std::string getSpriteStill(int equipo);

    std::string getSpriteRunning(int equipo);

    std::string getNombreJugador(int equipo, int jugador);

    std::string getSpriteSweeping(int equipo);

    std::string getSpriteKicking(int equipo);


    int getDefensores(int i);

    int getMediocampistas(int equipo);

    int getDelanteros(int equipo);

    std::string getLogLevel();

    void getSpriteBallStill();

    int validarUsuario(std::string user, std::string pass);

    int getMaxJugadores();
    
    int getAtaqueArqueroX();
    int getAtaqueDefensorX();
    int getAtaqueMediocampistaX();
    int getAtaqueDelanteroX();

    int getDefensaArqueroX();
    int getDefensaDefensorX();
    int getDefensaMediocampistaX();
    int getDefensaDelanteroX();
    
    int getAtaqueArqueroY();
    int getAtaqueDefensorY();
    int getAtaqueMediocampistaY();
    int getAtaqueDelanteroY();

    int getDefensaArqueroY();
    int getDefensaDefensorY();
    int getDefensaMediocampistaY();
    int getDefensaDelanteroY();

    int getDuracionTiempo();

    // C++11 singleton
    static YAMLReader& get_instance();
    YAMLReader(YAMLReader const&) = delete;
    void operator=(YAMLReader const&) = delete;

private:

    YAML::Node configNode;

    std::map<std::string,std::string> infoEquipo[3];

    FILE* files[4];

    std::string getFormacion(int equipo);

    std::map<std::string, std::string> cargarDefault();

    bool mapIsValid(std::map<std::string, std::string> map, std::vector<std::string> vector);

    bool cargarEquipo(bool equipo);

    std::string Formacion="3-2-1";

    void readFile(int archivo);

    void leerEquipo(std::string equipo, int posicionEnMapa);

    std::string getLogType(char *cadena);

    std::string Equipo="Local";
    std::string LogLevel="info";

    int maxJugadores = 4;

    int ataqueXArquero = 50;
    int ataqueXDefensor = 50;
    int ataqueXMediocampista = 50;
    int ataqueXDelantero = 50;

    int defensaXArquero = 50;
    int defensaXDefensor = 50;
    int defensaXMediocampista = 50;
    int defensaXDelantero = 50;

    int ataqueYArquero = 50;
    int ataqueYDefensor = 50;
    int ataqueYMediocampista = 50;
    int ataqueYDelantero = 50;

    int defensaYArquero = 50;
    int defensaYDefensor = 50;
    int defensaYMediocampista = 50;
    int defensaYDelantero = 50;

    int duracionTiempo = 60;
};


#endif //UNTITLED_YAMLREADER_H
