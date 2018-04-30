//
// Created by federico on 06/04/18.
//
#include <assert.h>
#include "YAMLReader.h"
#include "Log.h"
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <iterator>


YAMLReader* YAMLReader::instance = 0;

std::vector<std::string> separar(const std::string& str, const char& ch) ;

YAMLReader::YAMLReader() {
}

YAMLReader* YAMLReader::get_instance() {
    if (!(instance != 0)){
        instance= new YAMLReader();
    }
    return instance;
}

void YAMLReader::readYamlGeneral(std::string string){
    try {
        configNode = YAML::LoadFile("res/GeneralConfig.yaml");
    } catch (const std::exception& e){
        Log::initialize(LOG_ERROR);
        Log *log = Log::get_instance(); 
        log->error("Error al leer el archivo de configuracion general");
    }
    if (!Log::is_initialized()) {
        if(configNode["LogLevel"]){
            std::string logType = getLogType((char*)configNode["LogLevel"].as<std::string>().c_str());
            if(logType.compare("NOT_FOUND") == 0 ){
                Log::initialize(LOG_ERROR);
                Log* log=Log::get_instance();
                log->error("Log cargado en modo ERROR | No se encontro en la configuracion un nivel valido de log");
            } else {
                Log::initialize(logType);
                Log* log=Log::get_instance();
                log->error("Log cargado en modo " + configNode["LogLevel"].as<std::string>());

            }    
        }
    }

    leerEquipo("EquipoA", 1);
    leerEquipo("EquipoB", 2);

}

std::string YAMLReader::getNombre(int equipo){
    return infoEquipo[equipo]["Nombre"];
}

std::string YAMLReader::getFormacion(int equipo){
    return Formacion;
}


std::string YAMLReader::getSpriteStill(int equipo){
    return infoEquipo[equipo]["SpriteStill"];
}


std::string YAMLReader::getSpriteRunning(int equipo){
    return infoEquipo[equipo]["SpriteRunning"];
}

std::string YAMLReader::getSpriteKicking(int equipo){
    return infoEquipo[equipo]["SpriteKicking"];
}

std::string YAMLReader::getNombreJugador(int equipo, int jugador){
    return infoEquipo[equipo]["Jugador" + jugador];
}

std::string YAMLReader::getSpriteSweeping(int equipo) {
    return infoEquipo[equipo]["SpriteSweeping"];
}

int YAMLReader::getDefensores(int equipo) {
    return std::stoi(infoEquipo[equipo]["Defensores"]);
}

int YAMLReader::getMediocampistas(int equipo) {
    return std::stoi(infoEquipo[equipo]["Mediocampistas"]);
}

int YAMLReader::getDelanteros(int equipo) {
    return std::stoi(infoEquipo[equipo]["Delanteros"]);
}

std::string YAMLReader::getLogLevel() {
    return LogLevel;
}

std::vector<std::string> separar(const std::string& str, const char& ch) {
    std::string siguiente;
    std::vector<std::string> resultado;

    for (std::string::const_iterator it = str.begin(); it != str.end(); it++) {
        if (*it == ch) {
            if (!siguiente.empty()) {
                resultado.push_back(siguiente);
                siguiente.clear();
            }
        } else {
            siguiente += *it;
        }
    }
    if (!siguiente.empty())
        resultado.push_back(siguiente);
    return resultado;
}

std::string YAMLReader::getLogType(char *cadena) {
    std::string string(cadena);
    if( string.compare(LOG_ERROR)==0 || string.compare(LOG_DEBUG)==0 || string.compare(LOG_INFO)==0){
        return string;
    }
    return "NOT_FOUND";
}

void YAMLReader::leerEquipo(std::string equipo, int posicionEnMapa) {
    if(configNode[equipo]["Color"]){
        infoEquipo[posicionEnMapa]["SpriteStill"] = "res/" + configNode[equipo]["Color"].as<std::string>() + "/player_still.png";
        infoEquipo[posicionEnMapa]["SpriteRunning"] = "res/" + configNode[equipo]["Color"].as<std::string>() + "/player_run.png";
        infoEquipo[posicionEnMapa]["SpriteKicking"] = "res/" + configNode[equipo]["Color"].as<std::string>() + "/player_kick.png";
        infoEquipo[posicionEnMapa]["SpriteSweeping"] = "res/" + configNode[equipo]["Color"].as<std::string>() + "/player_sweep.png";
    } else {
        Log* log=Log::get_instance();
        log->error("No se encontro un color de camiseta para el " + equipo);
    }
    if(configNode[equipo]["Formacion"]){
        std::vector<std::string> resultado = separar(configNode[equipo]["Formacion"].as<std::string>(), '-');
        infoEquipo[posicionEnMapa]["Defensores"] = resultado[0];
        infoEquipo[posicionEnMapa]["Mediocampistas"] = resultado[1];
        infoEquipo[posicionEnMapa]["Delanteros"] = resultado[2];
    }else {
        Log* log=Log::get_instance();
        log->error("No se encontro formacion para el " + equipo);
        infoEquipo[posicionEnMapa]["Defensores"] = "3";
        infoEquipo[posicionEnMapa]["Mediocampistas"] = "2";
        infoEquipo[posicionEnMapa]["Delanteros"] = "1";
    }
}






