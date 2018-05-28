#include "GameMenu.h"

#include "common/GameConstants.h"

const int SCREEN_WIDTH = YAML::SCREEN_WIDTH;
const int SCREEN_HEIGHT = YAML::SCREEN_HEIGHT;

typedef std::chrono::steady_clock Clock;


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

    Texture background(gRenderer, "res/login_background.jpg");

    background.render(0,0);
    int tituloW, tituloH, usuarioW, usuarioH, passW, passH, errorH, errorW;

    EscrbiendoState usuarioOPass = EscrbiendoState::USUARIO;

    Texto tituloTexto(gRenderer, "res/Tehkan World Cup.ttf",36, "Tekhan Fiuba Cup", {255,255,0,0});
    Texto usuarioTexto(gRenderer, "res/Tehkan World Cup.ttf",22, "User: ", {255,255,0,0});
    Texto passTexto(gRenderer, "res/Tehkan World Cup.ttf",22, "Pass: ", {255,255,0,0});
    tituloTexto.getTextureDimensions(&tituloW,&tituloH); 
    usuarioTexto.getTextureDimensions(&usuarioW,&usuarioH); 
    passTexto.getTextureDimensions(&passW,&passH); 
    
    Texture indicatorRed(gRenderer, "res/write_here.png");
    Animation writeHereAnimation(indicatorRed, 2,4);
    
    std::string userText = "";
    Texto userTextTexture(gRenderer, "res/Tehkan World Cup.ttf",22,userText , {255,255,0,0});
    std::string passText = "";
    Texto passTextTexture(gRenderer, "res/Tehkan World Cup.ttf",22,passText , {255,255,0,0});

    std::string errorText = "";
    Texto errorTexto(gRenderer, "res/Tehkan World Cup.ttf",22,errorText , {255,0,0,0});

    log->info("Texturas del login creadas");

    SDL_StartTextInput();

    log->info("Comenzando modo lectura de texto");

    int returnValue = 0;

    bool running = true;
    bool loginTerminado = false;

    Clock::time_point currentTime, newTime;
    currentTime = Clock::now();
    std::chrono::milliseconds milli;
    const double fixed_dt = 0.25; 
    double accumulator = 0;
    double frametime;


    while ( running ) {
        SDL_Event ev;
        while ( SDL_PollEvent( &ev ) ) {
            if ( ev.type == SDL_TEXTINPUT ) {
                switch (usuarioOPass) {
                    case EscrbiendoState::USUARIO:
                        if(userText.compare("") == 0){
                            userText = ev.text.text;
                        } else {
                            userText += ev.text.text;
                        }
                    break;
                    case EscrbiendoState::PASS:
                        if(passText.compare("") == 0){
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
            } else if ( ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_BACKSPACE) {
                switch (usuarioOPass) {
                    case EscrbiendoState::USUARIO:
                        if(userText.size()) {
                            userText.pop_back();
                        }
                        break;
                        case EscrbiendoState::PASS:
                        if(passText.size()){
                            passText.pop_back();
                        }
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
                            userText = "";
                            passText = "";
                            errorText = "";
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
        tituloTexto.display((SCREEN_WIDTH - tituloW) / 2, ((SCREEN_HEIGHT - tituloH) / 4) + 20);
        usuarioTexto.display((SCREEN_WIDTH - usuarioW) / 3, (SCREEN_HEIGHT - usuarioH) / 2);
        passTexto.display((SCREEN_WIDTH - passW) / 3, ((SCREEN_HEIGHT - passH) / 2) + 70);

        userTextTexture.display(((SCREEN_WIDTH - usuarioW) / 3) + usuarioW, (SCREEN_HEIGHT - usuarioH) / 2);
        passTextTexture.display(((SCREEN_WIDTH - usuarioW) / 3) + passW, ((SCREEN_HEIGHT - passH) / 2) + 70);

        newTime = Clock::now();
        milli = std::chrono::duration_cast<std::chrono::milliseconds>(newTime - currentTime);
        currentTime = newTime;
        frametime = milli.count()/1000.0;

        accumulator += frametime;

        if (accumulator >= fixed_dt)
        {
            writeHereAnimation.update(fixed_dt);
            accumulator = 0;
        }
        
            switch (usuarioOPass) {
                case EscrbiendoState::USUARIO:
                    writeHereAnimation.render(((SCREEN_WIDTH - usuarioW) / 3) + usuarioW + (userText.size() * 22), (SCREEN_HEIGHT - usuarioH) / 2 - 10);
                    break;
                case EscrbiendoState::PASS:
                    writeHereAnimation.render(((SCREEN_WIDTH - usuarioW) / 3) + usuarioW + (passText.size() * 22), (SCREEN_HEIGHT - passH) / 2 + 60);
                    break;
                default:
                break;
            }


        if(loginTerminado) {
            log->info("Intento de ingreso~~Usuario: " + userText + " Password: " + passText);
            int respuestaAcceso = verificarCredenciales(userText, passText);
            if(respuestaAcceso == 0){
                log->info("Credenciales validas");
				short login_status = commandSender.login(userText + USERNAME_DELIMETER + passText);
				if (login_status == LOGIN_SUCCESS) {
	                log->info("Login aceptado por server");
                    SoundManager::get_instance()->playSound(SoundEffect::SE_OK);
	                running = false;
				} else if (!(login_status == LOGIN_ERROR)) {
	                log->info("Login no fue aceptado por server");
                    SoundManager::get_instance()->playSound(SoundEffect::SE_ERROR);
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
                SoundManager::get_instance()->playSound(SoundEffect::SE_ERROR);
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
                if(ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_p){
				    SoundManager::get_instance()->musicOn_off();
			    }
                if (ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_o) {
					SoundManager::get_instance()->soundEffectsOn_off();
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

int GameMenu::selectFormationScreen(CommandSender& commandSender) {
    Log *log = Log::get_instance();

    log->info("Generando pantalla de selección de formacion");

    Texture background(gRenderer, "res/choose_team.jpg");
    int tituloW, tituloH, fTxtH, fTxtW;

    Texto tituloTxt(gRenderer, "res/Tehkan World Cup.ttf",36, "ELEGIR FORMACION", {255,255,0,0});

    Texto f_321_txt(gRenderer, "res/Tehkan World Cup.ttf",28, "3-2-1", {255,255,0,0});
    Texto f_312_txt(gRenderer, "res/Tehkan World Cup.ttf",28, "3-1-2", {255,255,0,0});
    Texto f_330_txt(gRenderer, "res/Tehkan World Cup.ttf",28, "3-3-0", {255,255,0,0});

    Texture f_321_img(gRenderer, "res/formation_321.png");
    Texture f_312_img(gRenderer, "res/formation_312.png");
    Texture f_330_img(gRenderer, "res/formation_330.png");

    Texture selectTeam(gRenderer, "res/select_formation.png");

    Animation selectTeamAnimation(selectTeam, 2, 2);

    tituloTxt.getTextureDimensions(&tituloW,&tituloH);
    f_321_txt.getTextureDimensions(&fTxtW,&fTxtH); // Todas las formaciones tienen el mismo tamanio
        
    log->info("se crea las texturas para la selección de Formacion");

    int returnValue = 0;

    bool running = true;
    bool done = false;

    Clock::time_point currentTime, newTime;
    currentTime = Clock::now();
    std::chrono::milliseconds milli;
    const double fixed_dt = 0.5; 
    double accumulator = 0;
    double frametime;

    int selectFormationPosition_X[3];
    selectFormationPosition_X[0] = 20;
    selectFormationPosition_X[1] = (SCREEN_WIDTH / 3) + 20;
    selectFormationPosition_X[2] = ((SCREEN_WIDTH / 3) * 2)  + 20;
    int formationPositionIndex = 0;

    while ( running ) {
        SDL_Event ev;
        while ( SDL_PollEvent( &ev ) ) {
            if ( ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_LEFT) {
                if(formationPositionIndex != 0) {
                    SoundManager::get_instance()->playSound(SoundEffect::SE_SELECT);
                    formationPositionIndex--;
                }
            } else if ( ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_RIGHT) {
                if(formationPositionIndex != 2) {
                    SoundManager::get_instance()->playSound(SoundEffect::SE_SELECT);                
                    formationPositionIndex++;
                }
            } else if ( ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_RETURN) {
                SoundManager::get_instance()->playSound(SoundEffect::SE_OK);                
                done = true;
            } else if ( ev.type == SDL_QUIT ) {
                running = false;
                returnValue = -1;
            } else if (ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_p){
				SoundManager::get_instance()->musicOn_off();
			} else if (ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_o) {
                SoundManager::get_instance()->soundEffectsOn_off();
            }
        }

        SDL_RenderClear( gRenderer );
        background.render(0, 0);

        newTime = Clock::now();
        milli = std::chrono::duration_cast<std::chrono::milliseconds>(newTime - currentTime);
        currentTime = newTime;
        frametime = milli.count()/1000.0;

        accumulator += frametime;

        if (accumulator >= fixed_dt)
        {
            selectTeamAnimation.update(fixed_dt);
            accumulator = 0;
        }

       
        tituloTxt.display((SCREEN_WIDTH - tituloW) / 2, (SCREEN_HEIGHT - tituloH) / 8);

        int margenTextoFormacion = 62; // Ya hice la cuenta de cuento tiene que ser el margen para centrar textos de "3-2-1" de largo
        f_321_txt.display(margenTextoFormacion, SCREEN_HEIGHT / 4);
        f_312_txt.display((SCREEN_WIDTH / 3) + margenTextoFormacion, SCREEN_HEIGHT / 4);
        f_330_txt.display(((SCREEN_WIDTH / 3) * 2) + margenTextoFormacion, SCREEN_HEIGHT / 4);

        int margenImagenFormacion = 33; // Ya hice la cuenta con los pixel con formaciones de tamanio 200 x 320
        f_321_img.render(margenImagenFormacion, SCREEN_HEIGHT / 3);
        f_312_img.render((SCREEN_WIDTH / 3) + margenImagenFormacion, SCREEN_HEIGHT / 3);
        f_330_img.render(((SCREEN_WIDTH / 3) * 2) + margenImagenFormacion, SCREEN_HEIGHT / 3);

        selectTeamAnimation.render(selectFormationPosition_X[formationPositionIndex], (SCREEN_HEIGHT / 4) - 20);

        if(done) {
            std::string formacionElegida = "";
            switch (formationPositionIndex){
                case 0: 
                    formacionElegida = "3-2-1";
                    break;
                case 1:
                    formacionElegida = "3-1-2";
                    break;
                case 2 :
                    formacionElegida = "3-3-0";
                break;  
                default:
                    log->error("Hubo un error en la eleccion de la formacion, indice selccionado: " + formationPositionIndex);
                    formacionElegida = "3-2-1";
                break;

            }
            log->info("se selecciono la formacion: " + formacionElegida);
            returnValue = 0;
            running = false;
            //commandSender.assignTeam(team); ACA HAY QUE LLAMAR AL SERVIDOR PARA ASIGNAR LA FORMACION
        }
        SDL_RenderPresent( gRenderer );
    }

    return returnValue;
}
