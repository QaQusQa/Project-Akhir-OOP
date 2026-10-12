#ifndef MANAGER_H
#define MANAGER_H

#include <string>

class MANAGER {
public : 
static bool verifyName(const std::string &name);
static bool verifyRating(int& given_rating);
static bool verifyPlayTime(float& given_time);
static bool verifyPrice(float &time);
static bool verifyLaunchTarget(const std::string &target);
static bool verifyAchievement(const std::string &achievement);
};

#endif