#include "../include/Game.h"
#include <cmath>
#include <stdexcept>

string PCGAME::getPlatform() const { return "PC"; }
string MobileGAME::getPlatform() const { return "Mobile"; }
string ConsoleGAME::getPlatform() const { return "Console"; }
void GAME::setName(const string &game_name) {
  if (game_name.find_first_not_of(" \t\r\n") == string::npos)
    throw std::invalid_argument("Nama game harus diisi.");
  name = game_name;
}
void GAME::setPrice(float &game_price) {
  if (!std::isfinite(game_price) || game_price < 0)
    throw std::invalid_argument("Harga harus nol atau lebih.");
  price = game_price;
}
void GAME::setRating(int &rating_given) {
  if (rating_given < 0 || rating_given > 10)
    throw std::invalid_argument("Rating harus antara 0 dan 10.");
  rating = rating_given;
}
void GAME::setDescription(string &desc) { description = desc; }

float GAME::calcPlayTime(float &detected_time) {
  if (!std::isfinite(detected_time) || detected_time < 0 ||
      !std::isfinite(play_time + detected_time))
    throw std::invalid_argument("Waktu bermain tidak valid.");
  play_time += detected_time;
  return play_time;
}
int GAME::addAchievement(string &achivement) {
  if (achivement.find_first_not_of(" \t\r\n") == string::npos)
    throw std::invalid_argument("Achievement harus diisi.");
  achievements.push_back(achivement);
  return 1;
}
