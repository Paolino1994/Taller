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

    Texto *homeName;
    Texto *awayName;
    Texto *homeScore;
    Texto *awayScore;
	Texto *tiempo;
    Texto *goalText;
    Texto *goalKickText;

    GameManager *gameManager;

public:
	Score(SDL_Renderer *renderer, Texto *homeName, Texto *awayName, Texto *homeScore, Texto *awayScore, Texto *tiempo, Texto *goalText, Texto *goalKickText);

    ~Score();

	void initialize();

    void displayScore();

    void displayGoal();

    void displayGoalKick();

    string getStringTiempo(int time);
};

