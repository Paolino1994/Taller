#pragma once
#include <string>
#include <iostream>

#include "Texto.h"
#include "Texture.h"
#include "GameManager.h"
#include "SoundManager.h"
#include "common/GameConstants.h"
#include "common/YAMLReader.h"

class Score {

private:
	SDL_Renderer *gRenderer;

    int home;
    int away;
	int period;
	bool showingCountdown; // por un caso de borde uso esto

    Texto *homeName;
    Texto *awayName;
    Texto *homeScore;
    Texto *awayScore;
	Texto *periodText;
	Texto *tiempo;
    Texto *goalText;
    Texto *goalKickText;

    GameManager *gameManager;

public:
	Score(SDL_Renderer *renderer, Texto *homeName, Texto *awayName, Texto *homeScore, Texto *awayScore, Texto *periodText, Texto *tiempo, Texto *goalText, Texto *goalKickText);

    ~Score();

    void displayScore(int screen_x, int screen_y);

    void displayGoal();

    void displayGoalKick();

    string getStringTiempo(int time);
};

