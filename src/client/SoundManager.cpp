#include "SoundManager.h"


SoundManager* SoundManager::instance = 0;
bool SoundManager::initialized = false;

SoundManager::SoundManager(){
    music_on = true;
	gMusic = Mix_LoadMUS( "res/game_music.mp3" );
    if( gMusic == NULL )
    {
        Log::get_instance()->error("Failed to load beat music! SDL_mixer Error: " + (std::string)Mix_GetError());
    }
    loginOk = Mix_LoadWAV( "res/login_ok.wav" );
    if( loginOk == NULL )
    {
        Log::get_instance()->error("Failed to load beat music! SDL_mixer Error: " + (std::string)Mix_GetError());
    }
}

SoundManager::~SoundManager() {    
    //Free the music
    Mix_FreeMusic( gMusic );
    gMusic = NULL;

}

void SoundManager::initialize() {
    if (instance == 0) {
        instance = new SoundManager();
    } 
    initialized = true;
}

bool SoundManager::is_initialized() {
	return initialized;
}

SoundManager* SoundManager::get_instance() {
	if (instance == 0) {
		instance = new SoundManager();
	}
	return instance;
}

void SoundManager::musicOn_off(){
    music_on = music_on ? false : true;
    if(music_on) {
        this->startGameMusic();
    } else {
        this->pauseGameMusic();
    }
}

void SoundManager::startGameMusic(){
    //If there is no music playing
    if( Mix_PlayingMusic() == 0 )
    {
        //Play the music
        Mix_PlayMusic( gMusic, -1 );
    }
    //If the music is paused
    if( Mix_PausedMusic() == 1 )
    {
        //Resume the music
        Mix_ResumeMusic();
    }
}

void SoundManager::pauseGameMusic(){
    Mix_PauseMusic();
}

void SoundManager::playLoginOk(){
    Mix_PlayChannel( -1, loginOk, 0 );
}

