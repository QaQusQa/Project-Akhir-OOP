#ifndef GAME_H
#define GAME_H

#include <string>

using namespace std;

enum launcher { PC, Mobile, Console };

class GAME {
  string name;
  float price;
  float play_time;
  string achivements;

protected:
  string description;
  int rating;

public:
  void setName(string &game_name);
  void setPrice(float &game_price);
  void setRating(int &rating);
  void setDescription(string &desc);

  float calcPlayTime(float &detected_time);
  int addAchievement(string &achivement);
};

class PCGAME : public GAME {
  enum launcher launch = PC;
};

class MobileGAME : public GAME {
  enum launcher launch = Mobile;
};

class ConsoleGAME : public GAME {
  enum launcher launch = Console;
};

#endif