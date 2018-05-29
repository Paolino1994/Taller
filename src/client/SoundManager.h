#pragma once

#include <SDL_mixer.h>
#include <map>

#include "common/Log.h"

using namespace std;


enum SoundEffect {
    SE_OK = 0,
    SE_ERROR,
    SE_SELECT
};

class SoundManager
{
    private:
        Mix_Music *gMusic;
        std::map<SoundEffect, Mix_Chunk*> soundEffects;
        bool music_on = true;
        bool soundEffects_on = true;
	    static SoundManager* instance;

    static bool initialized;
        
    public:
        SoundManager();
        ~SoundManager();

        static void initialize();
	
	    static bool is_initialized();
	
	    static SoundManager* get_instance();

        void musicOn_off();

        void soundEffectsOn_off();

        void startGameMusic();

        void pauseGameMusic();

        void playSound(SoundEffect soundEffect);
};

