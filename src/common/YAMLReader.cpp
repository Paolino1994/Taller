//
// Created by federico on 06/04/18.
//
#include <assert.h>
#include "YAMLReader.h"
#include "common/Log.h"
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <iterator>
#include <algorithm>

std::vector<std::string> separar(const std::string& str, const char& ch) ;

YAMLReader::YAMLReader() {
}

YAMLReader& YAMLReader::get_instance() {
    static YAMLReader instance;
    return instance;
}

void YAMLReader::readYamlGeneral(std::string file){
    bool initFromFile = false;
    if (!file.empty()) {
        try {
            Log::get_instance()->error("Vamos a leer el archivo pasado por parametro: " + file);
            configNode = YAML::LoadFile(file);
            initFromFile = true;
        } catch (const std::exception& e){
            Log *log = Log::get_instance(); 
            log->error("Error al leer el archivo de configuracion pasado por parametro");
        }
    }

    const std::string defaultConfig = "res/GeneralConfig.yaml";

    if (!initFromFile) {
        try {
            Log::get_instance()->error("Vamos a leer el archivo de configuracion por defecto, actualmente es: " + defaultConfig);
            configNode = YAML::LoadFile(defaultConfig);
        } catch (const std::exception& e){
            Log *log = Log::get_instance(); 
            log->error("Error al leer el archivo de configuracion por defecto");
        }
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

    if(configNode["MaxJugadores"]){
        int maxJugadoresAux = std::stoi(configNode["MaxJugadores"].as<std::string>());
        if(maxJugadoresAux >= 1 && maxJugadoresAux <= 4) {
            maxJugadores = maxJugadoresAux;
        } else {
            Log* log=Log::get_instance();
            log->error("Valor invalido ingresado en el archivo de configutacion: MaxJugadores " + configNode["MaxJugadores"].as<std::string>() + " se utilizara el valor 4");
            maxJugadores = 4;
        }
    }

    if(configNode["MovJugAtaqueX"]){
        if(configNode["MovJugAtaqueX"]["Arquero"]){
            int aux = std::stoi(configNode["MovJugAtaqueX"]["Arquero"].as<std::string>());
            if (aux > 0 ){
                ataqueXArquero = aux;
            } else {
                Log* log=Log::get_instance();
                log->error("Valor invalido ingresado en el archivo de configutacion: AtaqueArquero " + configNode["MovJugAtaqueX"]["Arquero"].as<std::string>() + " se utilizara el valor 50");
                ataqueXArquero = 50;
            }
        }
        if(configNode["MovJugAtaqueX"]["Defensores"]){
            int aux = std::stoi(configNode["MovJugAtaqueX"]["Defensores"].as<std::string>());
            if (aux > 0 ){
                ataqueXDefensor = aux;
            } else {
                Log* log=Log::get_instance();
                log->error("Valor invalido ingresado en el archivo de configutacion: Defensores " + configNode["MovJugAtaqueX"]["Defensores"].as<std::string>() + " se utilizara el valor 50");
                ataqueXDefensor = 50;
            }
        }
        if(configNode["MovJugAtaqueX"]["Mediocampistas"]){
            int aux = std::stoi(configNode["MovJugAtaqueX"]["Mediocampistas"].as<std::string>());
            if (aux > 0 ){
                ataqueXMediocampista = aux;
            } else {
                Log* log=Log::get_instance();
                log->error("Valor invalido ingresado en el archivo de configutacion: Mediocampistas " + configNode["MovJugAtaqueX"]["Mediocampistas"].as<std::string>() + " se utilizara el valor 50");
                ataqueXDefensor = 50;
            }
        }
        if(configNode["MovJugAtaqueX"]["Delanteros"]){
            int aux = std::stoi(configNode["MovJugAtaqueX"]["Delanteros"].as<std::string>());
            if (aux > 0 ){
                ataqueXDelantero = aux;
            } else {
                Log* log=Log::get_instance();
                log->error("Valor invalido ingresado en el archivo de configutacion: Delantero " + configNode["MovJugAtaqueX"]["Delanteros"].as<std::string>() + " se utilizara el valor 50");
                ataqueXDefensor = 50;
            }
        }
    }

    if(configNode["MovJugDefensaX"]){
        if(configNode["MovJugDefensaX"]["Arquero"]){
            int aux = std::stoi(configNode["MovJugDefensaX"]["Arquero"].as<std::string>());
            if (aux > 0 ){
                defensaXArquero = aux;
            } else {
                Log* log=Log::get_instance();
                log->error("Valor invalido ingresado en el archivo de configutacion: DefensaArquero " + configNode["MovJugDefensaX"]["Arquero"].as<std::string>() + " se utilizara el valor 50");
                defensaXArquero = 50;
            }
        }
        if(configNode["MovJugDefensaX"]["Defensores"]){
            int aux = std::stoi(configNode["MovJugDefensaX"]["Defensores"].as<std::string>());
            if (aux > 0 ){
                defensaXDefensor = aux;
            } else {
                Log* log=Log::get_instance();
                log->error("Valor invalido ingresado en el archivo de configutacion: Defensores " + configNode["MovJugDefensaX"]["Defensores"].as<std::string>() + " se utilizara el valor 50");
                defensaXDefensor = 50;
            }
        }
        if(configNode["MovJugDefensaX"]["Mediocampistas"]){
            int aux = std::stoi(configNode["MovJugDefensaX"]["Mediocampistas"].as<std::string>());
            if (aux > 0 ){
                defensaXMediocampista = aux;
            } else {
                Log* log=Log::get_instance();
                log->error("Valor invalido ingresado en el archivo de configutacion: Mediocampistas " + configNode["MovJugDefensaX"]["Mediocampistas"].as<std::string>() + " se utilizara el valor 50");
                defensaXDefensor = 50;
            }
        }
        if(configNode["MovJugDefensaX"]["Delanteros"]){
            int aux = std::stoi(configNode["MovJugDefensaX"]["Delanteros"].as<std::string>());
            if (aux > 0 ){
                defensaXDelantero = aux;
            } else {
                Log* log=Log::get_instance();
                log->error("Valor invalido ingresado en el archivo de configutacion: Delantero " + configNode["MovJugDefensaX"]["Delanteros"].as<std::string>() + " se utilizara el valor 50");
                defensaXDefensor = 50;
            }
        }
    }

     if(configNode["MovJugAtaqueY"]){
        if(configNode["MovJugAtaqueY"]["Arquero"]){
            int aux = std::stoi(configNode["MovJugAtaqueY"]["Arquero"].as<std::string>());
            if (aux > 0 ){
                ataqueYArquero = aux;
            } else {
                Log* log=Log::get_instance();
                log->error("Valor invalido ingresado en el archivo de configutacion: AtaqueArquero " + configNode["MovJugAtaqueY"]["Arquero"].as<std::string>() + " se utilizara el valor 50");
                ataqueYArquero = 50;
            }
        }
        if(configNode["MovJugAtaqueY"]["Defensores"]){
            int aux = std::stoi(configNode["MovJugAtaqueY"]["Defensores"].as<std::string>());
            if (aux > 0 ){
                ataqueYDefensor = aux;
            } else {
                Log* log=Log::get_instance();
                log->error("Valor invalido ingresado en el archivo de configutacion: Defensores " + configNode["MovJugAtaqueY"]["Defensores"].as<std::string>() + " se utilizara el valor 50");
                ataqueYDefensor = 50;
            }
        }
        if(configNode["MovJugAtaqueY"]["Mediocampistas"]){
            int aux = std::stoi(configNode["MovJugAtaqueY"]["Mediocampistas"].as<std::string>());
            if (aux > 0 ){
                ataqueYMediocampista = aux;
            } else {
                Log* log=Log::get_instance();
                log->error("Valor invalido ingresado en el archivo de configutacion: Mediocampistas " + configNode["MovJugAtaqueY"]["Mediocampistas"].as<std::string>() + " se utilizara el valor 50");
                ataqueYDefensor = 50;
            }
        }
        if(configNode["MovJugAtaqueY"]["Delanteros"]){
            int aux = std::stoi(configNode["MovJugAtaqueY"]["Delanteros"].as<std::string>());
            if (aux > 0 ){
                ataqueYDelantero = aux;
            } else {
                Log* log=Log::get_instance();
                log->error("Valor invalido ingresado en el archivo de configutacion: Delantero " + configNode["MovJugAtaqueY"]["Delanteros"].as<std::string>() + " se utilizara el valor 50");
                ataqueYDefensor = 50;
            }
        }
    }

    if(configNode["MovJugDefensaY"]){
        if(configNode["MovJugDefensaY"]["Arquero"]){
            int aux = std::stoi(configNode["MovJugDefensaY"]["Arquero"].as<std::string>());
            if (aux > 0 ){
                defensaYArquero = aux;
            } else {
                Log* log=Log::get_instance();
                log->error("Valor invalido ingresado en el archivo de configutacion: DefensaArquero " + configNode["MovJugDefensaY"]["Arquero"].as<std::string>() + " se utilizara el valor 50");
                defensaYArquero = 50;
            }
        }
        if(configNode["MovJugDefensaY"]["Defensores"]){
            int aux = std::stoi(configNode["MovJugDefensaY"]["Defensores"].as<std::string>());
            if (aux > 0 ){
                defensaYDefensor = aux;
            } else {
                Log* log=Log::get_instance();
                log->error("Valor invalido ingresado en el archivo de configutacion: Defensores " + configNode["MovJugDefensaY"]["Defensores"].as<std::string>() + " se utilizara el valor 50");
                defensaYDefensor = 50;
            }
        }
        if(configNode["MovJugDefensaY"]["Mediocampistas"]){
            int aux = std::stoi(configNode["MovJugDefensaY"]["Mediocampistas"].as<std::string>());
            if (aux > 0 ){
                defensaYMediocampista = aux;
            } else {
                Log* log=Log::get_instance();
                log->error("Valor invalido ingresado en el archivo de configutacion: Mediocampistas " + configNode["MovJugDefensaY"]["Mediocampistas"].as<std::string>() + " se utilizara el valor 50");
                defensaYDefensor = 50;
            }
        }
        if(configNode["MovJugDefensaY"]["Delanteros"]){
            int aux = std::stoi(configNode["MovJugDefensaY"]["Delanteros"].as<std::string>());
            if (aux > 0 ){
                defensaYDelantero = aux;
            } else {
                Log* log=Log::get_instance();
                log->error("Valor invalido ingresado en el archivo de configutacion: Delantero " + configNode["MovJugDefensaY"]["Delanteros"].as<std::string>() + " se utilizara el valor 50");
                defensaYDefensor = 50;
            }
        }
    }


    leerEquipo("EquipoA", 1);
    leerEquipo("EquipoB", 2);

}

std::string YAMLReader::getTeamNombre(int equipo){
    return infoEquipo[equipo]["Nombre"];
}

std::string YAMLReader::getFormacion(int equipo){
    return Formacion;
}

std::string YAMLReader::getTeamColor(int equipo){
    return infoEquipo[equipo]["Color"];
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

int YAMLReader::getMaxJugadores() {
    return maxJugadores;
}

int YAMLReader::getAtaqueArqueroX() {
    return ataqueXArquero;
}

int YAMLReader::getAtaqueDefensorX() {
    return ataqueXDefensor;
}

int YAMLReader::getAtaqueMediocampistaX() {
    return ataqueXMediocampista;
}

int YAMLReader::getAtaqueDelanteroX() {
    return ataqueXDelantero;
}

int YAMLReader::getDefensaArqueroX() {
    return defensaXArquero;
}

int YAMLReader::getDefensaDefensorX() {
    return defensaXDefensor;
}

int YAMLReader::getDefensaMediocampistaX() {
    return defensaXMediocampista;
}

int YAMLReader::getDefensaDelanteroX() {
    return defensaXDelantero;
}

int YAMLReader::getAtaqueArqueroY() {
    return ataqueYArquero;
}

int YAMLReader::getAtaqueDefensorY() {
    return ataqueYDefensor;
}

int YAMLReader::getAtaqueMediocampistaY() {
    return ataqueYMediocampista;
}

int YAMLReader::getAtaqueDelanteroY() {
    return ataqueYDelantero;
}

int YAMLReader::getDefensaArqueroY() {
    return defensaYArquero;
}

int YAMLReader::getDefensaDefensorY() {
    return defensaYDefensor;
}

int YAMLReader::getDefensaMediocampistaY() {
    return defensaYMediocampista;
}

int YAMLReader::getDefensaDelanteroY() {
    return defensaYDelantero;
}

// 0 -> Ok, -1 Password incorrecta, -2 usuario no encontrado
int YAMLReader::validarUsuario(std::string user, std::string pass) {
    std::vector<std::string> separados;
    for (std::size_t i=0;i<configNode["usuarios"].size() -1 ;i++) {
        std::string userPassString = configNode["usuarios"][i].as<std::string>();
        std::string::iterator end_pos = std::remove(userPassString.begin(), userPassString.end(), ' ');
        userPassString.erase(end_pos, userPassString.end());
        separados = separar(userPassString, '-');
        if(user.compare(separados[0]) == 0) {
            if(pass.compare(separados[1]) == 0) {
                return 0;
            } else {
                return -1;
            }
        }
    }
    return -2;
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

void checkFormacion(std::vector<std::string>& formacion) {
	
	bool isOk = true;
	
	if (formacion.size() > 3) {
		Log::get_instance()->error("Encontramos una formaci�n de mas de 3 lineas");
		isOk = false;
	}
	else { // llenar formacion con "0" si faltan definir
		while (formacion.size() < 3) {
			formacion.push_back("0");
		}
	}
	
	if (isOk) {
		for (std::string& cantidad : formacion) {
			try {
				std::stoi(cantidad); //no puede ser negativo porque splitteamos por el signo -
			}
			catch (const std::exception&) {
				Log::get_instance()->error("El valor: " + cantidad + " de la formacion no es un numero");
				isOk = false;
				break;
			}
		}
	}

	if (!isOk) {
		Log::get_instance()->error("Por errores en la formaci�n, vamos con una por defecto: 3-2-1");
		formacion[0] = "3";
		formacion[1] = "2";
		formacion[2] = "1";
	}

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
        infoEquipo[posicionEnMapa]["Color"] = configNode[equipo]["Color"].as<std::string>();
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
		checkFormacion(resultado); //TEMP, ver de poner en otro lado (o no, no importa mucho)
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
    if(configNode[equipo]["Nombre"]){
        infoEquipo[posicionEnMapa]["Nombre"] = configNode[equipo]["Nombre"].as<std::string>();
    } else {
        Log* log=Log::get_instance();
        log->error("No se encontro nombre para el " + equipo);
        infoEquipo[posicionEnMapa]["Nombre"] = equipo;        
    }

}






