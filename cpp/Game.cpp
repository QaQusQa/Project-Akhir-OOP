#include "../include/Game.h"

string PCGAME::getPlatform() const { return "PC"; }
string MobileGAME::getPlatform() const { return "Mobile"; }
string ConsoleGAME::getPlatform() const { return "Console"; }
void GAME::setName(string &game_name) { name = game_name; }
void GAME::setPrice(float &game_price) { price = game_price; }
void GAME::setRating(int &rating_given) { rating = rating_given; }
void GAME::setDescription(string &desc) { description = desc; }

float GAME::calcPlayTime(float &detected_time) {
  play_time = detected_time;
  return play_time;
}
int GAME::addAchievement(string &achivement) {
  achivements = achivement;
  return 1;
}
