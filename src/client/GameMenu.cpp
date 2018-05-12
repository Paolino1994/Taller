#include "GameMenu.h"

#include "common/GameConstants.h"

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

int GameMenu::logginScreen(CommandSender& commandSender) {
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
				short login_status = commandSender.login(userText + USERNAME_DELIMETER + passText);
				if (login_status == LOGIN_SUCCESS) {
	                log->info("Login aceptado por server");
	                running = false;
				} else if (!(login_status == LOGIN_ERROR)) {
	                log->info("Login no fue aceptado por server");
		            usuarioOPass = EscrbiendoState::ERROR;
		            userText = " ";
		            passText = " ";
					loginTerminado = false;
					switch (login_status) {
						case WRONG_CREDENTIALS:
							errorText = "Credenciales invalidas";
							break;
						case GAME_FULL:
							errorText = "Juego completo";
							break;
						case ALREADY_LOGGED_IN:
							errorText = "Usuario conectado en otro dispositvo";
							break;
						case GAME_ALREADY_STARTED:
							errorText = "El juego ya está empezado";
							break;
						default:
							errorText = "Error en login";
					}
				} else {
					return -1;
				}
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

int GameMenu::verificarCredenciales (std::string usuario, std::string pass) {
    int valor = YAMLReader::get_instance().validarUsuario(usuario,pass);
    return valor;
}
