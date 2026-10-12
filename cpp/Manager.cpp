#include "../include/Manager.h"
#include <cmath>

bool MANAGER::verifyName(const std::string &name) {
    return name.find_first_not_of(" \t\r\n") != std::string::npos;
}

bool MANAGER::verifyPrice(float &price) {
    return std::isfinite(price) && price >= 0.0f;
}

bool MANAGER::verifyRating(int &rating) {
    return rating >= 0 && rating <= 10;
}

bool MANAGER::verifyPlayTime(float &time) {
    return std::isfinite(time) && time >= 0.0f;
}

bool MANAGER::verifyLaunchTarget(const std::string &target) {
    return target.find_first_not_of(" \t\r\n") != std::string::npos;
}

bool MANAGER::verifyAchievement(const std::string &achievement) {
    return achievement.find_first_not_of(" \t\r\n") != std::string::npos;
}