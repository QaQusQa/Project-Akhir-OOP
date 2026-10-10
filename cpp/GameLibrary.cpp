#include "../include/GameLibrary.h"
#include <stdexcept>

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

void GameLibrary::addGame(const string &name, launcher platform, const string &launchTarget) {
  unique_ptr<GAME> game = createGame(platform);
  if (!game) throw std::invalid_argument("Platform tidak valid.");
  game->setName(name);
  if (!launchTarget.empty()) game->setLaunchTarget(launchTarget);
  games.push_back(std::move(game));
}

void GameLibrary::editGame(int index, const string &name, launcher platform, const string &launchTarget) {
  if (index < 0 || index >= static_cast<int>(games.size())) return;
  unique_ptr<GAME> game = createGame(platform);
  if (!game) throw std::invalid_argument("Platform tidak valid.");
  if (launchTarget.empty() && game->getPlatform() == games[index]->getPlatform())
    game->setLaunchTarget(games[index]->getLaunchTarget());
  static_cast<GAME &>(*game) = *games[index];
  game->setName(name);
  if (!launchTarget.empty()) game->setLaunchTarget(launchTarget);
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
  if (!getGame(index)) return;
  auto updated = createGame(PC);
  static_cast<GAME &>(*updated) = *games[index];
  updated->setPrice(price);
  updated->setRating(rating);
  updated->setDescription(description);
  static_cast<GAME &>(*games[index]) = *updated;
}

void GameLibrary::calcPlayTime(int index, float hours) {
  if (getGame(index)) games[index]->calcPlayTime(hours);
}

void GameLibrary::addAchievement(int index, string achievement) {
  if (getGame(index)) games[index]->addAchievement(achievement);
}

const GAME *GameLibrary::getGame(int index) const {
  if (index < 0 || index >= static_cast<int>(games.size())) return nullptr;
  return games[index].get();
}
