#include "Score.h"
#include "../common/GameConstants.h"


Score::Score(SDL_Renderer *renderer, Texto *homeName, Texto *awayName, Texto *homeScore, Texto *awayScore, Texto *periodText, Texto *tiempo, Texto *goalText, Texto *goalKickText):
	gRenderer(renderer),
	home(GameManager::get_instance()->getScore(Team::HOME)),
	away(GameManager::get_instance()->getScore(Team::AWAY)),
	period(GameManager::get_instance()->getPeriod()),
	showingCountdown(false),
	homeName(homeName),
	awayName(awayName),
	homeScore(homeScore),
	awayScore(awayScore),
	periodText(periodText),
	tiempo(tiempo),
	goalText(goalText),
	goalKickText(goalKickText),
	gameManager(GameManager::get_instance())
{
}

Score::~Score()
{
}

void Score::displayScore(int screen_x, int screen_y){

	// Scores

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

	homeName->display(screen_x + 30, screen_y + 10);
	awayName->display(screen_x + 180, screen_y + 10);
	homeScore->display(screen_x + 30, screen_y + 60);
	awayScore->display(screen_x + 180, screen_y + 60);

	// Time
	
	if (GameManager::get_instance()->getRestartCountdownSecondsRemaining() > 0) {
		this->showingCountdown = true;
		periodText->updateText("Revancha en", { 255,255,0,0 });
	}

	if (gameManager->getPeriod() != this->period) {
		this->showingCountdown = false; // si cambia de tiempo es porque esta andando;
		this->period = gameManager->getPeriod();
		if (this->period == 1) {
			this->periodText->updateText("1er Tiempo", { 255,255,0,0 });
		} else if (this->period == 2) {
			this->periodText->updateText("2do Tiempo", { 255,255,0,0 });
		}
	}

	if (this->period != 0) {
		std::string Tiempo;
		int periodTextXOffset;
		if (this->showingCountdown) {
			Tiempo = getStringTiempo(GameManager::get_instance()->getRestartCountdownSecondsRemaining());
			periodTextXOffset = 540;
		}
		else {
			Tiempo = getStringTiempo(newTime);
			periodTextXOffset = 560;
		}

		tiempo->updateText(Tiempo, { 255,255,0,0 });
		periodText->display(screen_x + periodTextXOffset, screen_y + 10);
		tiempo->display(screen_x + 540, screen_y + 60);
	}
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

