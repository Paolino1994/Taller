#include "Score.h"
#include "../common/GameConstants.h"


Score::Score(SDL_Renderer *renderer, Texto *homeName, Texto *awayName, Texto *homeScore, Texto *awayScore,Texto *tiempo, Texto *goalText, Texto *goalKickText):
gRenderer(renderer),
homeName(homeName),
awayName(awayName),
homeScore(homeScore),
awayScore(awayScore),
tiempo(tiempo),
goalText(goalText),
goalKickText(goalKickText)
{
    home = 0;
    away = 0;
    tiempo=0;
    gameManager = GameManager::get_instance();
}

Score::~Score()
{
}

void Score::displayScore(int screen_x, int screen_y){

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

    homeName->display(screen_x + 30, screen_y + 10);
    awayName->display(screen_x + 180, screen_y + 10);
    homeScore->display(screen_x + 30, screen_y + 60);
    awayScore->display(screen_x + 180, screen_y + 60);
    tiempo->display(screen_x + 550, screen_y + 60);
}

void Score::displayGoal(){
    goalText->display(135,350);
}

void Score::displayGoalKick(){
    goalKickText->display(200,400);
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

