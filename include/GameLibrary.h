#ifndef GAME_LIBRARY_H
#define GAME_LIBRARY_H

#include "Game.h"
#include <vector>
#include <memory>

class GameLibrary {
  std::vector<std::unique_ptr<GAME>> games;
  std::unique_ptr<GAME> createGame(launcher platform) const;

public:
  void addGame(const std::string &name, launcher platform);
  void editGame(int index, const std::string &name, launcher platform);
  void deleteGame(int index);
  void updateDetails(int index, float price, int rating, std::string description);
  void addPlayTime(int index, float hours);
  void addAchievement(int index, std::string achievement);
  std::vector<std::string> showGames() const;
  const GAME *getGame(int index) const;
};

#endif // GAME_LIBRARY_H
