#ifndef STORAGE_H
#define STORAGE_H

#include "../include/Game.h"

#include <memory>
#include <string>
#include <vector>
using namespace std;

class STORAGE {
  string filePath;

public:
  explicit STORAGE(const string &path = "games.json");
  bool saveGames(const vector<unique_ptr<GAME>> &games);
  bool loadGames(vector<unique_ptr<GAME>> &games);
};

#endif