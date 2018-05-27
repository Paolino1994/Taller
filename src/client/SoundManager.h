#pragma once

#include <SDL_mixer.h>

#include "common/Log.h"

class SoundManager
{
    private:
        Mix_Music *gMusic;
        Mix_Chunk *loginOk;
        bool music_on = true;
	    static SoundManager* instance;

    static bool initialized;
        
    public:
        SoundManager();
        ~SoundManager();

        static void initialize();
	
	    static bool is_initialized();
	
	    static SoundManager* get_instance();

        void musicOn_off();

        void startGameMusic();

        void pauseGameMusic();

        void playLoginOk();

};

