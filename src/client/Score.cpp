#include "Score.h"

const int SCREEN_WIDTH = YAML::SCREEN_WIDTH;
const int SCREEN_HEIGHT = YAML::SCREEN_HEIGHT;

Score::Score(SDL_Renderer *renderer, Texto *homeName, Texto *awayName, Texto *homeScore, Texto *awayScore):
gRenderer(renderer),
homeName(homeName),
awayName(awayName),
homeScore(homeScore),
awayScore(awayScore)
{
    home = 0;
    away = 0;
    gameManager = GameManager::get_instance();
}

Score::~Score()
{
}

void Score::initialize()
{
    
}

void Score::displayScore(){

    int newScoreHome = gameManager->getScore(Team::HOME);
    int newScoreAway = gameManager->getScore(Team::AWAY);

    if(newScoreHome != home) {
        homeScore->updateText(to_string(newScoreHome), {255,255,0,0});
    }

    if(newScoreAway != away) {
        awayScore->updateText(to_string(newScoreAway), {255,255,0,0});
    }

    homeName->display(30,10);
    awayName->display(180, 10);
    homeScore->display(30, 60);
    awayScore->display(180, 60);
}

