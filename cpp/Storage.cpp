#include "../include/Storage.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <qjsondocument.h>

STORAGE::STORAGE(const string &path) : filePath(path) {}

bool STORAGE::saveGames(const vector<unique_ptr<GAME>> &games) {
  QJsonArray array;
  for (const auto &game : games) {
    QJsonObject object;
    object["name"] = QString::fromStdString(game->getName());
    object["platform"] = QString::fromStdString(game->getPlatform());
    object["launch_target"] = QString::fromStdString(game->getLaunchTarget());
    object["price"] = game->getPrice();
    object["rating"] = game->getRating();
    object["play_time"] = game->getPlayTime();
    object["description"] = QString::fromStdString(game->getDescription());

    QJsonArray achivementArray;
    for (const auto &ach : game->getAchievements()) {
      achivementArray.append(QString::fromStdString(ach));
    }
    object["achievements"] = achivementArray;
    array.append(object);
  }
  QFile file(QString::fromStdString(filePath));
  if (!file.open(QIODevice::WriteOnly))
    return false;

  QJsonDocument doc(array);
  file.write(doc.toJson(QJsonDocument::Indented));
  return true;
}

bool STORAGE::loadGames(vector<unique_ptr<GAME>> &games) {
  QFile file(QString::fromStdString(filePath));
  if (!file.exists() || !file.open(QIODevice::ReadOnly))
    return false;

  QByteArray data = file.readAll();
  QJsonDocument doc = QJsonDocument::fromJson(data);
  if (!doc.isArray())
    return false;

  games.clear();
  QJsonArray array = doc.array();

  for (const auto &value : array) {
    QJsonObject object = value.toObject();
    string platform = object["platform"].toString().toStdString();
    unique_ptr<GAME> game;
    if (platform == "PC")
      game = make_unique<PCGAME>();
    else if (platform == "Mobile")
      game = make_unique<MobileGAME>();
    else if (platform == "Console")
      game = make_unique<ConsoleGAME>();
    else
      continue;

    game->setName(object["name"].toString().toStdString());
    game->setLaunchTarget(object["launch_target"].toString().toStdString());

    float price = static_cast<float>(object["price"].toDouble());
    int rating = static_cast<int>(object["rating"].toInt());

    string desc = object["description"].toString().toStdString();
    float playTime = static_cast<float>(object["play_time"].toDouble());

    game->setPrice(price);
    game->setRating(rating);
    game->setDescription(desc);
    game->calcPlayTime(playTime);

    QJsonArray achievementsArray = object["achievements"].toArray();
    for (const auto &val : achievementsArray) {
      string ach = val.toString().toStdString();
      game->addAchievement(ach);
    }
    games.push_back(std::move(game));
  }

  return true;
}