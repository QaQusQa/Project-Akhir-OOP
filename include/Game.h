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
  virtual string getLaunchInstructions() const = 0;
  virtual const string &getLaunchTarget() const = 0;
  virtual void setLaunchTarget(const string &target) = 0;

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
  string pcLauncher = "Steam";

public:
  string getPlatform() const override;
  string getLaunchInstructions() const override;
  const string &getLaunchTarget() const override { return pcLauncher; }
  void setLaunchTarget(const string &target) override;
};

class MobileGAME : public GAME {
  string operatingSystem = "Android";

public:
  string getPlatform() const override;
  string getLaunchInstructions() const override;
  const string &getLaunchTarget() const override { return operatingSystem; }
  void setLaunchTarget(const string &target) override;
};

class ConsoleGAME : public GAME {
  string consoleName = "PlayStation 5";

public:
  string getPlatform() const override;
  string getLaunchInstructions() const override;
  const string &getLaunchTarget() const override { return consoleName; }
  void setLaunchTarget(const string &target) override;
};

#endif
