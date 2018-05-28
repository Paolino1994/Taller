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
    Mix_Chunk* okSound = Mix_LoadWAV( "res/ok_sound.wav" );
    if( okSound == NULL )
    {
        Log::get_instance()->error("Failed to load beat music! SDL_mixer Error: " + (std::string)Mix_GetError());
    }
    soundEffects.insert ( std::pair<SoundEffect,Mix_Chunk*>(SoundEffect::SE_OK,okSound) );
    Mix_Chunk* errorSound = Mix_LoadWAV( "res/error_sound.wav" );
    if( errorSound == NULL )
    {
        Log::get_instance()->error("Failed to load beat music! SDL_mixer Error: " + (std::string)Mix_GetError());
    }
    soundEffects.insert ( std::pair<SoundEffect,Mix_Chunk*>(SoundEffect::SE_ERROR,errorSound) );
    Mix_Chunk* selectSound = Mix_LoadWAV( "res/select_sound.wav" );
    if( selectSound == NULL )
    {
        Log::get_instance()->error("Failed to load beat music! SDL_mixer Error: " + (std::string)Mix_GetError());
    }
    soundEffects.insert ( std::pair<SoundEffect,Mix_Chunk*>(SoundEffect::SE_SELECT,selectSound) );
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

void SoundManager::playSound(SoundEffect soundEffect){
    Mix_PlayChannel( -1, soundEffects.at(soundEffect), 0 );
}
