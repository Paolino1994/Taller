//
// Created by federico on 25/04/18.
//

#include "TextureSetter.h"




TextureSetter::TextureSetter(int i, SDL_Renderer *pRenderer) {
    equipo=i;
    gRenderer=pRenderer;
    setInfos();

}

Texture TextureSetter::setTextureRun() {

    Log::get_instance()->info(YAMLReader::get_instance().getSpriteRunning(equipo));
    Surface runS(PlayerRun.file_path);
    runS.setColorKey(126, 130, 56); //cargar desde constantes
    Texture run(gRenderer, runS);
    run.setScaling(PlayerRun.width, PlayerRun.height);
    return run;
}

Texture TextureSetter::setTextureStill() {

    Surface stillS(PlayerStill.file_path);
    stillS.setColorKey(126, 130, 56); //cargar desde constantes
    Texture still(gRenderer, stillS);
    still.setScaling(PlayerStill.width, PlayerStill.height);
    return still;
}

Texture TextureSetter::setTextureSweep() {

    Surface sweepS(PlayerSweep.file_path);
    sweepS.setColorKey(126, 130, 56); //cargar desde constantes
    Texture sweep(gRenderer, sweepS);
    sweep.setScaling(PlayerSweep.width, PlayerSweep.height);
    return sweep;
}

Texture TextureSetter::setTextureKick() {

    Surface kickS(PlayerKick.file_path);
    kickS.setColorKey(126, 130, 56); //cargar desde constantes
    Texture kick(gRenderer, kickS);
    kick.setScaling(PlayerKick.width, PlayerKick.height);
    return kick;
}

sprite_info TextureSetter::getPlayerRunInfo() {
    return PlayerRun;
}

Texture TextureSetter::getPLayerRunTexture() {
    return setTextureRun();
}

sprite_info TextureSetter::getPlayerStillInfo() {
    return PlayerStill;
}

Texture TextureSetter::getPLayerStillTexture() {
    return setTextureStill();
}

sprite_info TextureSetter::getPlayerSweepInfo() {
    return PlayerSweep;
}

Texture TextureSetter::getPLayerSweepTexture() {
    return setTextureSweep();
}

sprite_info TextureSetter::getPlayerKickInfo() {
    return PlayerKick;
}

Texture TextureSetter::getPLayerKickTexture() {
    return setTextureKick();
}

void TextureSetter::setInfos() {
    PlayerRun={"run",YAMLReader::get_instance().getSpriteRunning(equipo),60, 64,4,12};
    PlayerStill={"still",YAMLReader::get_instance().getSpriteStill(equipo),68, 34,1,3};
    PlayerSweep={"sweep",YAMLReader::get_instance().getSpriteSweeping(equipo),76, 112,4,12};
    PlayerKick={"kick",YAMLReader::get_instance().getSpriteKicking(equipo),74, 94,4,12};

}
