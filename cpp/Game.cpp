#include "../include/Game.h"

string PCGAME::getPlatform() const { return "PC"; }
string MobileGAME::getPlatform() const { return "Mobile"; }
string ConsoleGAME::getPlatform() const { return "Console"; }
void GAME::setName(const string &game_name) { name = game_name; }
void GAME::setPrice(float &game_price) { price = game_price; }
void GAME::setRating(int &rating_given) { rating = rating_given; }
void GAME::setDescription(string &desc) { description = desc; }

float GAME::calcPlayTime(float &detected_time) {
  play_time = detected_time;
  return play_time;
}
int GAME::addAchievement(string &achivement) {
  if (achivement.find_first_not_of(" \t\r\n") == string::npos) return 0;
  achievements.push_back(achivement);
  return 1;
}

void GAME::addPlayTime(float hours) {
  if (hours > 0) play_time += hours;
}
