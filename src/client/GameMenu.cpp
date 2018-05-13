#include "GameMenu.h"

const int SCREEN_WIDTH = YAML::SCREEN_WIDTH;
const int SCREEN_HEIGHT = YAML::SCREEN_HEIGHT;


enum EscrbiendoState {
    USUARIO = 0,
    PASS,
    ERROR
};

GameMenu::GameMenu(SDL_Renderer *renderer):
    gRenderer(renderer)
    {

}

int GameMenu::logginScreen() {
    Log *log = Log::get_instance();

    log->info("Generando pantalla de login");

    Surface surface("res/soccer_field_1.6.png");

    Texture background(gRenderer, "res/soccer_field_1.6.png");

    background.render( SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
    int tituloW, tituloH, usuarioW, usuarioH, passW, passH, errorH, errorW;

    EscrbiendoState usuarioOPass = EscrbiendoState::USUARIO;

    Texto tituloTexto(gRenderer, "res/Tehkan World Cup.ttf",36, "Tekhan Fiuba Cup", {255,255,0,0});
    Texto usuarioTexto(gRenderer, "res/Tehkan World Cup.ttf",22, "User: ", {255,255,0,0});
    Texto passTexto(gRenderer, "res/Tehkan World Cup.ttf",22, "Pass: ", {255,255,0,0});
    tituloTexto.getTextureDimensions(&tituloW,&tituloH); 
    usuarioTexto.getTextureDimensions(&usuarioW,&usuarioH); 
    passTexto.getTextureDimensions(&passW,&passH); 
    
    std::string userText = " ";
    Texto userTextTexture(gRenderer, "res/Tehkan World Cup.ttf",22,userText , {255,255,0,0});
    std::string passText = " ";
    Texto passTextTexture(gRenderer, "res/Tehkan World Cup.ttf",22,passText , {255,255,0,0});

    std::string errorText = " ";
    Texto errorTexto(gRenderer, "res/Tehkan World Cup.ttf",22,errorText , {255,0,0,0});

    log->info("Texturas del login creadas");

    SDL_StartTextInput();

    log->info("Comenzando modo lectura de texto");

    int returnValue = 0;

    bool running = true;
    bool loginTerminado = false;
    while ( running ) {
        SDL_Event ev;
        while ( SDL_PollEvent( &ev ) ) {
            if ( ev.type == SDL_TEXTINPUT ) {
                switch (usuarioOPass) {
                    case EscrbiendoState::USUARIO:
                        if(userText.compare(" ") == 0){
                            userText = ev.text.text;
                        } else {
                            userText += ev.text.text;
                        }
                    break;
                    case EscrbiendoState::PASS:
                        if(passText.compare(" ") == 0){
                            passText = ev.text.text;
                        } else {
                            passText += ev.text.text;
                        }
                    break;
                    case EscrbiendoState::ERROR:
                    break;
                    default:
                    break;
                }
            } else if ( ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_BACKSPACE && userText.size()) {
                switch (usuarioOPass) {
                    case EscrbiendoState::USUARIO:
                            userText.pop_back();
                        break;
                        case EscrbiendoState::PASS:
                            passText.pop_back();
                        break;
                        case EscrbiendoState::ERROR:
                        break;
                        default:
                        break;
                }
            } else if ( ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_RETURN) {
                switch (usuarioOPass) {
                    case EscrbiendoState::USUARIO:
                            usuarioOPass = EscrbiendoState::PASS;
                        break;
                        case EscrbiendoState::PASS:
                            loginTerminado = true;
                        break;
                        case EscrbiendoState::ERROR:
                            usuarioOPass = EscrbiendoState::USUARIO;
                            userText = " ";
                            passText = " ";
                            errorText = " ";
                        default:
                        break;
                }
            } else if ( ev.type == SDL_QUIT ) {
                running = false;
                returnValue = -1;
            }
        }
        
        SDL_RenderClear( gRenderer );
        background.render(0, 0);
        if(usuarioOPass == EscrbiendoState::USUARIO) {
            userTextTexture.updateText(userText, {255,0,0,0});
            passTextTexture.updateText(passText, {255,255,0,0});
        } else if (usuarioOPass == EscrbiendoState::PASS){
            userTextTexture.updateText(userText, {255,255,0,0});
            passTextTexture.updateText(passText, {255,0,0,0});
        } else if (usuarioOPass == EscrbiendoState::ERROR){
            errorTexto.updateText(errorText, {255,0,0,0});
            errorTexto.getTextureDimensions(&errorW,&errorH); 
            errorTexto.display((SCREEN_WIDTH - errorW) / 2, ((SCREEN_HEIGHT - errorH) / 4) * 3 );
        }
        tituloTexto.display((SCREEN_WIDTH - tituloW) / 2, (SCREEN_HEIGHT - tituloH) / 4);
        usuarioTexto.display((SCREEN_WIDTH - usuarioW) / 3, (SCREEN_HEIGHT - usuarioH) / 2);
        passTexto.display((SCREEN_WIDTH - passW) / 3, ((SCREEN_HEIGHT - passH) / 2) + 70);

        userTextTexture.display(((SCREEN_WIDTH - usuarioW) / 3) + usuarioW, (SCREEN_HEIGHT - usuarioH) / 2);
        passTextTexture.display(((SCREEN_WIDTH - usuarioW) / 3) + passW, ((SCREEN_HEIGHT - passH) / 2) + 70);

        if(loginTerminado) {
            log->info("Intento de ingreso~~Usuario: " + userText + " Password: " + passText);
            int respuestaAcceso = verificarCredenciales(userText, passText);
            if(respuestaAcceso == 0){
                log->info("Credenciales validas");
                running = false;
            } else {
                if (respuestaAcceso == -1) {
                    log->info("Password incorrecto");
                    errorText = "Password invalido, presione enter.";
                }
                if (respuestaAcceso == -2) {
                    log->info("Usuario incorrecto");
                    errorText = "Usuario invalido, presione enter.";
                }
                usuarioOPass = EscrbiendoState::ERROR;
                userText = " ";
                passText = " ";
                loginTerminado = false;
            }
        } 
        SDL_RenderPresent( gRenderer );
    }

    SDL_StopTextInput();
    log->info("Modo de lectura de texto terminado");

    return returnValue;
}

bool GameMenu::pausaMenu(SDL_Event ev){
    Log *log = Log::get_instance();
    Surface surface("res/soccer_field_1.6.png");

    Texture background(gRenderer, "res/soccer_field_1.6.png");

    background.render(0, 0);
    // Agrego mensaje para salir del juego
    log->info("Cargar Mensaje salida de juego");
    Texto quiereSalirTexto(gRenderer, "res/Tehkan World Cup.ttf",36, "SALIR DEL JUEGO? S/N", {255,255,0,0});
    bool confirmaSalir = false;
    bool seleccionoLetraValida = false;
    int w, h;
    quiereSalirTexto.getTextureDimensions(&w,&h); // pregunto el tamanio para el centrado
    quiereSalirTexto.display((SCREEN_WIDTH - w) / 2, (SCREEN_HEIGHT - h) / 2); // muestro la pregunta centrada
    while(!seleccionoLetraValida){
        while( SDL_PollEvent( &ev ) != 0)
            {
                SDL_RenderClear( gRenderer );
                if ( ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_s ) {
                    log->info("Salida del juego confirmada");
                    confirmaSalir = true;
                    seleccionoLetraValida = true;
                }
                if ( ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_n ) { // Con n vuelve al juego
                    log->info("Salida del juego cancelada");
                    confirmaSalir = false;
                    seleccionoLetraValida = true;
                }
            }
        background.render(0, 0);
        quiereSalirTexto.display((SCREEN_WIDTH - w) / 2, (SCREEN_HEIGHT - h) / 2); // muestro la pregunta centrada
        //SDL_RenderClear( gRenderer );
        SDL_RenderPresent( gRenderer );
    }
    return confirmaSalir;
}

int GameMenu::verificarCredenciales (std::string usuario, std::string pass) {
    int valor = YAMLReader::get_instance().validarUsuario(usuario,pass);
    return valor;
}
