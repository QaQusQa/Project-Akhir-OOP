#include "../include/GameLibrary.h"

using namespace std;

unique_ptr<GAME> GameLibrary::createGame(launcher platform) const {
  switch (platform) {
  case PC:
    return make_unique<PCGAME>();
  case Mobile:
    return make_unique<MobileGAME>();
  case Console:
    return make_unique<ConsoleGAME>();
  }
  return nullptr;
}

void GameLibrary::addGame(const string &name, launcher platform) {
  if (name.find_first_not_of(" \t\r\n") == string::npos) return;
  unique_ptr<GAME> game = createGame(platform);
  if (!game) return;
  game->setName(name);
  games.push_back(std::move(game));
}

void GameLibrary::editGame(int index, const string &name, launcher platform) {
  if (index < 0 || index >= static_cast<int>(games.size())) return;
  if (name.find_first_not_of(" \t\r\n") == string::npos) return;
  unique_ptr<GAME> game = createGame(platform);
  if (!game) return;
  // Pertahankan data game lama saat platform diganti.
  static_cast<GAME &>(*game) = *games[index];
  game->setName(name);
  games[index] = std::move(game);
}

void GameLibrary::deleteGame(int index) {
  if (index < 0 || index >= static_cast<int>(games.size())) return;
  games.erase(games.begin() + index);
}

vector<string> GameLibrary::showGames() const {
  vector<string> gameNames;
  for (const auto &game : games) {
    gameNames.push_back(game->getName() + " (" + game->getPlatform() + ")");
  }
  return gameNames;
}

void GameLibrary::updateDetails(int index, float price, int rating, string description) {
  if (!getGame(index) || price < 0 || rating < 0 || rating > 10) return;
  games[index]->setPrice(price);
  games[index]->setRating(rating);
  games[index]->setDescription(description);
}

void GameLibrary::addPlayTime(int index, float hours) {
  if (getGame(index)) games[index]->addPlayTime(hours);
}

void GameLibrary::addAchievement(int index, string achievement) {
  if (getGame(index)) games[index]->addAchievement(achievement);
}

const GAME *GameLibrary::getGame(int index) const {
  if (index < 0 || index >= static_cast<int>(games.size())) return nullptr;
  return games[index].get();
}
