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

YAMLReader::YAMLReader(std::string string) {
    Log::initialize(LOG_INFO);
    Log* log=Log::get_instance();

    log->info("Intento abrir Archivo de Configuracion General");
    files[3]=startEquipo(string,3);
    log->info("Abierto Archivo de Configuracion General");
    log->info("Intento abrir Archivo de Equipo"+EQUIPO1);
    files[EQUIPO1]= startEquipo(const_cast<char *>(find("Equipo1", 3).c_str()), EQUIPO1);
    log->info("Abierto Archivo de Equipo"+EQUIPO1);
    log->info("Intento abrir Archivo de Equipo"+EQUIPO2);
    files[EQUIPO2]= startEquipo(const_cast<char *>(find("Equipo2", 3).c_str()), EQUIPO2);
    log->info("Abierto Archivo de Equipo"+EQUIPO2);
    log->info("Intento destruir archivo de configuracion general");
    destroyFile(3);
    log->info("Destrui archivo de configuracion general");
    log->info("Intento leer archivos de configuracion de equipos");
    readArchives();
    log->info("Lei archivos de configuracion de equipos");
    destroy();
}

YAMLReader* YAMLReader::get_instance() {
    if (!(instance != 0)){
        instance= new YAMLReader("GeneralConfig.yaml");
    }
    return instance;
}


std::string YAMLReader::getNombre(int equipo){


    return infoEquipo[equipo]["Nombre"];
}

std::string YAMLReader::getFormacion(int equipo){
    return infoEquipo[equipo]["Formacion"];
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





void YAMLReader::readArchives() {
    infoEquipo[EQUIPO1]=getEverything(EQUIPO1);
    infoEquipo[EQUIPO2]=getEverything(EQUIPO2);
}



void YAMLReader::destroy() {
    for(int i=0;i<2;i++){
        destroyFile(i);
    }
}

void YAMLReader::destroyFile(int i) {
    yaml_parser_delete(&parser[i]);
    //fclose(files[i]);
}



std::string YAMLReader::getSpritesEquipo(int equipo) {
    std::string ret= find("Sprite",equipo);
    return ret;
}

void YAMLReader::printType(yaml_token_type_t e) {
    switch(e)
    {
        case YAML_STREAM_START_TOKEN: puts("STREAM Inicio"); break;
        case YAML_STREAM_END_TOKEN:   puts("STREAM Fin");   break;

        case YAML_KEY_TOKEN:   printf("Key   "); break;
        case YAML_VALUE_TOKEN: printf("Value "); break;

        case YAML_BLOCK_SEQUENCE_START_TOKEN: puts("<b>Bloque secuencia</b>"); break;
        case YAML_BLOCK_ENTRY_TOKEN:          puts("<b>Bloque Entrada</b>");    break;
        case YAML_BLOCK_END_TOKEN:            puts("<b>Fin de Bloque</b>");              break;

        case YAML_BLOCK_MAPPING_START_TOKEN:  puts("Mapeo de bloque");            break;
        case YAML_SCALAR_TOKEN:  printf("Valor %s \n", token.data.scalar.value); break;

        default:
            printf("Token tipo  %d\n", token.type);
    }


}





void YAMLReader::printAll(int arch) {
    do {
        yaml_parser_scan(&parser[arch], &token);
        printType(token.type);
        if(token.type != YAML_STREAM_END_TOKEN)
            yaml_token_delete(&token);
    } while(token.type != YAML_STREAM_END_TOKEN);
    yaml_token_delete(&token);
}

void readToken(yaml_token_t tok) {
    yaml_token_type_t e=tok.type;
    switch(e)
    {
        case YAML_STREAM_START_TOKEN: puts("STREAM Inicio"); break;
        case YAML_STREAM_END_TOKEN:   puts("STREAM Fin");   break;

        case YAML_KEY_TOKEN:   printf("Key   "); break;
        case YAML_VALUE_TOKEN: printf("Value "); break;

        case YAML_BLOCK_SEQUENCE_START_TOKEN: puts("<b>Bloque secuencia</b>"); break;
        case YAML_BLOCK_ENTRY_TOKEN:          puts("<b>Bloque Entrada</b>");    break;
        case YAML_BLOCK_END_TOKEN:            puts("<b>Fin de Bloque</b>");              break;

        case YAML_BLOCK_MAPPING_START_TOKEN:  puts("Mapeo de bloque");            break;
        case YAML_SCALAR_TOKEN:  printf("Valor %s \n", tok.data.scalar.value); break;

        default:
            printf("Token tipo  %d\n", tok.type);
    }

}

void YAMLReader::readAll(int arch) {
    do {
        yaml_parser_scan(&parser[arch], &token);
        readToken(token);
        if(token.type != YAML_STREAM_END_TOKEN)
            yaml_token_delete(&token);
    } while(token.type != YAML_STREAM_END_TOKEN);
    yaml_token_delete(&token);
}


FILE * YAMLReader::startEquipo(std::string equipo, int arch) {
    std::string ruta="res/";
    std::string rutaEquipo= ruta + equipo;
    files[arch]=fopen(rutaEquipo.c_str(), "r");
    if(!yaml_parser_initialize(&(parser[arch])))
        fputs("Error de parser\n", stderr);
    if(files[arch] == NULL)
        fputs("Error al abrir un archivo!\n", stderr);
    yaml_parser_set_input_file(&(parser[arch]), files[arch]);
    return files[arch];

}

std::string YAMLReader::find(std::string string, int equipo) {
    do {
        yaml_parser_scan(&parser[equipo], &token);
        //printType(token.type);
        if(token.type==YAML_SCALAR_TOKEN) {
            if (string.compare((char*)token.data.scalar.value)==0) {
                yaml_token_delete(&token);
                while (1) {
                    //printType(token.type);
                    if (token.type == YAML_SCALAR_TOKEN) {
                        return std::string((char*)token.data.scalar.value);
                        break;
                    }else{
                        if (token.type != YAML_STREAM_END_TOKEN) {
                            yaml_token_delete(&token);
                        }else{
                            break;
                        }
                    }
                    yaml_parser_scan(&parser[equipo], &token);
                    //std::cout << "Nombre " << reinterpret_cast<const char *>(token.data.scalar.value) << std::endl;

                }
            }
        }
        if(token.type != YAML_STREAM_END_TOKEN)
            yaml_token_delete(&token);
    } while(token.type != YAML_STREAM_END_TOKEN);
    yaml_token_delete(&token);
    return "Error";
}



std::map<std::string, std::string> YAMLReader::getEverything(int equipo) {
    std::map<std::string,std::string> mapa;
    Log* log=Log::get_instance();
    std::vector<std::string> keys={"Nombre","Formacion","SpriteStill","SpriteRunning","SpriteSweeping","SpriteKicking","Jugador1","Jugador2","Jugador3","Jugador4","Jugador5","Jugador6"};
    int i=0;
    bool error=false;
    for(i=0;i<6;i++){
        //std::cout<<i<<std::endl;
        log->info(keys[i]);
        mapa[keys[i]]=find(keys[i], equipo);
        log->info(mapa[keys[i]]);
        if(mapa[keys[i]].compare("Error")==0){
            error=true;
            log->info("Error en la lectura");
            break;
        }
    }
    for(i=6;i<12;i++){
        if(error==true){
            break;
        }
        log->info(keys[i]);
        mapa[keys[i]]=findJugador(keys[i], equipo);
    }

    if (mapIsValid(mapa,keys)){
        std::stringstream info;
        info <<"Archivo de informacion Equipo" << equipo +1 <<".yaml es valido";
        log->info(info.str());
        return mapa;
    }else{
        std::stringstream info;
        info <<"Archivo de informacion Equipo" << equipo +1<<".yaml no es valido";
        log->info(info.str());
        log->info(cargarDefault()["Nombre"]);
        return cargarDefault();

    }

}

std::string YAMLReader::findJugador(std::string string, int equipo) {
    find(string,equipo);
    findnext(equipo);
    //return find("Nombre",equipo);
    return std::string((char*)token.data.scalar.value);
}

std::string YAMLReader::findnext(int equipo) {
    do{
        yaml_token_delete(&token);
        yaml_parser_scan(&parser[equipo], &token);
        if (token.type == YAML_SCALAR_TOKEN) {
            return std::string((char*)token.data.scalar.value);
            break;
        }else{
            if (token.type != YAML_STREAM_END_TOKEN) {
                yaml_token_delete(&token);
            }else{
                break;
            }
        }

    }while(token.type != YAML_STREAM_END_TOKEN);
    yaml_token_delete(&token);
    return "Error";


}

std::map<std::string, std::string> YAMLReader::cargarDefault() {
    std::map<std::string, std::string> mapa;
    //std::vector<std::string> keys={"Nombre","Formacion","SpriteStill","SpriteRunning","SpriteSweeping","SpriteKicking","Jugador1","Jugador2","Jugador3","Jugador4","Jugador5","Jugador6"};
    mapa["Nombre"]="Default";
    mapa["Formacion"]="2-3-0";
    mapa["SpriteStill"]="res/player_still.png";
    mapa["SpriteRunning"]="res/player_run.png";
    mapa["SpriteSweeping"]="res/player_sweep.png";
    mapa["SpriteKicking"]="res/player_kick.png";
    mapa["Jugador1"]="Default1";
    mapa["Jugador2"]="Default2";
    mapa["Jugador3"]="Default3";
    mapa["Jugador4"]="Default4";
    mapa["Jugador5"]="Default5";
    mapa["Jugador6"]="Default6";
    return mapa;

}

bool YAMLReader::mapIsValid(std::map<std::string, std::string> map, std::vector<std::string> keys) {
    int i;
    for(i=0;i<(int)keys.size();i++){
        if(map[keys[i]].compare("Error")==0){
            return false;
        }
    }
    return true;
}

int YAMLReader::getDefensores(int equipo) {
    std::string string = getFormacion(equipo);
    std::vector<std::string> result = separar(string, '-');
    return std::stoi(result[0]);

}

int YAMLReader::getMediocampistas(int equipo) {
    std::string string = getFormacion(equipo);
    std::vector<std::string> result = separar(string, '-');
    return std::stoi(result[1]);
}

int YAMLReader::getDelanteros(int equipo) {
    std::string string = getFormacion(equipo);
    std::vector<std::string> result = separar(string, '-');
    return std::stoi(result[2]);
}

std::vector<std::string> separar(const std::string& str, const char& ch) {
    std::string siguiente;
    std::vector<std::string> resultado;

    // For each character in the string
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





