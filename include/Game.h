#ifndef GAME_H
#define GAME_H

#include <string>
#include <vector>

using namespace std;

enum launcher { PC, Mobile, Console };

class GAME {
  string name;
  float price = 0;
  float play_time = 0;
  vector<string> achievements;

protected:
  string description;
  int rating = 0;

public:
  virtual ~GAME() = default;
  virtual string getPlatform() const = 0;

  const string &getName() const { return name; }
  float getPrice() const { return price; }
  float getPlayTime() const { return play_time; }
  int getRating() const { return rating; }
  const string &getDescription() const { return description; }
  const vector<string> &getAchievements() const { return achievements; }
  void setName(const string &game_name);
  void setPrice(float &game_price);
  void setRating(int &rating);
  void setDescription(string &desc);

  float calcPlayTime(float &detected_time);
  int addAchievement(string &achivement);
};

class PCGAME : public GAME {
public:
  string getPlatform() const override;
};

class MobileGAME : public GAME {
public:
  string getPlatform() const override;
};

class ConsoleGAME : public GAME {
public:
  string getPlatform() const override;
};

#endif
