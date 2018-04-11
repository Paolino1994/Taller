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


void readToken(yaml_token_t e);



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

std::string YAMLReader::getNombreJugador(int equipo, int jugador){
    return infoEquipo[equipo]["Jugador" + jugador];
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
        /* Stream start/end */
        case YAML_STREAM_START_TOKEN: puts("STREAM START"); break;
        case YAML_STREAM_END_TOKEN:   puts("STREAM END");   break;
            /* Token types (read before actual token) */
        case YAML_KEY_TOKEN:   printf("(Key token)   "); break;
        case YAML_VALUE_TOKEN: printf("(Value token) "); break;
            /* Block delimeters */
        case YAML_BLOCK_SEQUENCE_START_TOKEN: puts("<b>Start Block (Sequence)</b>"); break;
        case YAML_BLOCK_ENTRY_TOKEN:          puts("<b>Start Block (Entry)</b>");    break;
        case YAML_BLOCK_END_TOKEN:            puts("<b>End block</b>");              break;
            /* Data */
        case YAML_BLOCK_MAPPING_START_TOKEN:  puts("[Block mapping]");            break;
        case YAML_SCALAR_TOKEN:  printf("scalar %s \n", token.data.scalar.value); break;
            /* Others */
        default:
            printf("Got token of type %d\n", token.type);
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

void YAMLReader::readAll(int arch) {
    do {
        yaml_parser_scan(&parser[arch], &token);
        readToken(token);
        if(token.type != YAML_STREAM_END_TOKEN)
            yaml_token_delete(&token);
    } while(token.type != YAML_STREAM_END_TOKEN);
    yaml_token_delete(&token);
}

void readToken(yaml_token_t tok) {
    yaml_token_type_t e=tok.type;
    switch(e)
    {
        /* Stream start/end */
        case YAML_STREAM_START_TOKEN: puts("STREAM START"); break;
        case YAML_STREAM_END_TOKEN:   puts("STREAM END");   break;
            /* Token types (read before actual token) */
        case YAML_KEY_TOKEN:   printf("(Key token)   "); break;
        case YAML_VALUE_TOKEN: printf("(Value token) "); break;
            /* Block delimeters */
        case YAML_BLOCK_SEQUENCE_START_TOKEN: puts("<b>Start Block (Sequence)</b>"); break;
        case YAML_BLOCK_ENTRY_TOKEN:          puts("<b>Start Block (Entry)</b>");    break;
        case YAML_BLOCK_END_TOKEN:            puts("<b>End block</b>");              break;
            /* Data */
        case YAML_BLOCK_MAPPING_START_TOKEN:  puts("[Block mapping]");            break;
        case YAML_SCALAR_TOKEN:  printf("scalar %s \n", tok.data.scalar.value); break;
            /* Others */
        default:
            printf("Got token of type %d\n", tok.type);
    }

}




FILE * YAMLReader::startEquipo(std::string equipo, int arch) {
    std::string ruta="res/";
    std::string rutaEquipo= ruta + equipo;
    //std::cout<<asd<<std::endl;
    files[arch]=fopen(rutaEquipo.c_str(), "r");
    /* Initialize parser */
    if(!yaml_parser_initialize(&(parser[arch])))
        fputs("Failed to initialize parser!\n", stderr);

    if(files[arch] == NULL)
        fputs("Failed to open file!\n", stderr);

    /* Set input file */
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
    return NULL;
}



std::map<std::string, std::string> YAMLReader::getEverything(int equipo) {
    std::map<std::string,std::string> mapa;
    std::vector<std::string> keys={"Nombre","Formacion","SpriteStill","SpriteRunning","Jugador1","Jugador2","Jugador3","Jugador4","Jugador5","Jugador6"};
    //std::cout<<"HOLAsad"<<std::endl;
    int i=0;
    for(i=0;i<4;i++){
        mapa[keys[i]]=find(keys[i], equipo);
    }
    for(i=4;i<10;i++){
        mapa[keys[i]]=findJugador(keys[i], equipo);
    }



    return mapa;

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
    return NULL;


}



