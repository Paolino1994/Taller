#include "Score.h"
#include "../common/GameConstants.h"

const int SCREEN_WIDTH = YAML::SCREEN_WIDTH;
const int SCREEN_HEIGHT = YAML::SCREEN_HEIGHT;

Score::Score(SDL_Renderer *renderer, Texto *homeName, Texto *awayName, Texto *homeScore, Texto *awayScore,Texto *tiempo):
gRenderer(renderer),
homeName(homeName),
awayName(awayName),
homeScore(homeScore),
awayScore(awayScore),
tiempo(tiempo)
{
    home = 0;
    away = 0;
    tiempo=0;
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
    int newTime = gameManager->getTime();

    if(newScoreHome != home) {
        homeScore->updateText(to_string(newScoreHome), {255,255,0,0});
        home = newScoreHome;
    }

    if(newScoreAway != away) {
        awayScore->updateText(to_string(newScoreAway), {255,255,0,0});
        away = newScoreAway;
    }

    std::string Tiempo=getStringTiempo(newTime);
    tiempo->updateText(Tiempo, {255,255,0,0});

    homeName->display(30,10);
    awayName->display(180, 10);
    homeScore->display(30, 60);
    awayScore->display(180, 60);
    tiempo->display(550,60);
}

string Score::getStringTiempo(int time) {
    std::string devolver="";
    int seconds=time%60;
    int minutes=time/60;
    if(minutes>=10){
        devolver=std::to_string(minutes);
    }else {
        devolver = "0" + std::to_string(minutes);
    }
    if(seconds>=10){
        return devolver+":"+std::to_string(seconds);
    }else{
        return devolver+":0"+std::to_string(seconds);
    }

}

