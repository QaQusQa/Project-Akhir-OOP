#include "../include/GameLibrary.h"
#include <QApplication>
#include <QComboBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QSpinBox>
#include <QStackedWidget>
#include <QTextEdit>
#include <QVBoxLayout>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  GameLibrary library;
  QWidget window;
  window.setWindowTitle("Game Library");
  window.resize(500, 400);
  QVBoxLayout layout(&window);
  QLabel title("Game Library");
  title.setAlignment(Qt::AlignCenter);
  layout.addWidget(&title);
  QStackedWidget pages;
  layout.addWidget(&pages);

  QWidget home;
  QVBoxLayout menu(&home);
  QPushButton add("Add Game"), show("Show Games"), exit("Keluar");
  menu.addStretch();
  menu.addWidget(&add);
  menu.addWidget(&show);
  menu.addWidget(&exit);
  menu.addStretch();
  pages.addWidget(&home);

  QWidget collection;
  QVBoxLayout collectionLayout(&collection);
  QLabel hint("Pilih game untuk melihat atau mengedit detailnya.");
  QListWidget list;
  QPushButton edit("Detail / Edit"), remove("Delete Game"), back("Kembali");
  QHBoxLayout actions;
  actions.addWidget(&edit);
  actions.addWidget(&remove);
  collectionLayout.addWidget(&hint);
  collectionLayout.addWidget(&list);
  collectionLayout.addLayout(&actions);
  collectionLayout.addWidget(&back);
  pages.addWidget(&collection);

  auto refresh = [&]() {
    int row = list.currentRow();
    list.clear();
    for (const auto &game : library.showGames())
      list.addItem(QString::fromStdString(game));
    if (list.count() > 0) list.setCurrentRow(row >= 0 && row < list.count() ? row : 0);
    edit.setEnabled(list.count() > 0);
    remove.setEnabled(list.count() > 0);
    hint.setText(list.count() ? "Pilih game untuk melihat atau mengedit detailnya."
                             : "Belum ada game. Tambahkan lewat menu utama.");
  };

  auto gameForm = [&](int index) {
    const GAME *game = library.getGame(index);
    QDialog dialog(&window);
    dialog.setWindowTitle(game ? "Detail / Edit Game" : "Add Game");
    dialog.resize(400, 300);
    QFormLayout form(&dialog);
    QLineEdit name;
    QComboBox platform;
    platform.addItems({"PC", "Mobile", "Console"});
    QDoubleSpinBox price;
    price.setRange(0, 1000000000);
    price.setPrefix("Rp ");
    QSpinBox rating;
    rating.setRange(0, 10);
    rating.setSuffix(" / 10");
    QTextEdit description;
    description.setMaximumHeight(80);
    form.addRow("Nama", &name);
    form.addRow("Platform", &platform);
    form.addRow("Harga", &price);
    form.addRow("Rating", &rating);
    form.addRow("Deskripsi", &description);

    QLabel totalTime;
    QDoubleSpinBox extraTime;
    extraTime.setRange(0, 100000);
    extraTime.setSuffix(" jam");
    QPushButton addTime("Tambah");
    QHBoxLayout timeRow;
    timeRow.addWidget(&extraTime);
    timeRow.addWidget(&addTime);
    QListWidget achievements;
    achievements.setMaximumHeight(100);
    QLineEdit achievement;
    QPushButton addAchievement("Tambah");
    QHBoxLayout achievementRow;
    achievementRow.addWidget(&achievement);
    achievementRow.addWidget(&addAchievement);
    float pendingHours = 0;
    std::vector<std::string> pendingAchievements;

    if (game) {
      name.setText(QString::fromStdString(game->getName()));
      platform.setCurrentText(QString::fromStdString(game->getPlatform()));
      price.setValue(game->getPrice());
      rating.setValue(game->getRating());
      description.setPlainText(QString::fromStdString(game->getDescription()));
      totalTime.setText(QString::number(game->getPlayTime()) + " jam");
      for (const auto &item : game->getAchievements())
        achievements.addItem(QString::fromStdString(item));
      form.addRow("Total waktu bermain", &totalTime);
      form.addRow("Tambah waktu", &timeRow);
      form.addRow("Achievement", &achievements);
      form.addRow("Achievement baru", &achievementRow);
    }

    QObject::connect(&addTime, &QPushButton::clicked, [&]() {
      pendingHours += static_cast<float>(extraTime.value());
      totalTime.setText(QString::number(game->getPlayTime() + pendingHours) + " jam");
      extraTime.setValue(0);
    });
    QObject::connect(&addAchievement, &QPushButton::clicked, [&]() {
      QString text = achievement.text().trimmed();
      if (text.isEmpty()) return;
      pendingAchievements.push_back(text.toStdString());
      achievements.addItem(text);
      achievement.clear();
    });

    QDialogButtonBox buttons(QDialogButtonBox::Save | QDialogButtonBox::Cancel);
    form.addRow(&buttons);
    QObject::connect(&buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    QObject::connect(&buttons, &QDialogButtonBox::accepted, [&]() {
      if (name.text().trimmed().isEmpty()) {
        QMessageBox::information(&dialog, "Nama game", "Nama game harus diisi.");
        return;
      }
      dialog.accept();
    });
    if (dialog.exec() != QDialog::Accepted) return;

    auto gameName = name.text().trimmed().toStdString();
    auto selectedPlatform = static_cast<launcher>(platform.currentIndex());
    if (game) library.editGame(index, gameName, selectedPlatform);
    else {
      index = static_cast<int>(library.showGames().size());
      library.addGame(gameName, selectedPlatform);
    }
    library.updateDetails(index, static_cast<float>(price.value()), rating.value(),
                          description.toPlainText().toStdString());
    library.addPlayTime(index, pendingHours);
    for (const auto &item : pendingAchievements) library.addAchievement(index, item);
    refresh();
  };

  QObject::connect(&add, &QPushButton::clicked, [&]() { gameForm(-1); });
  QObject::connect(&show, &QPushButton::clicked, [&]() {
    refresh();
    pages.setCurrentIndex(1);
  });
  QObject::connect(&back, &QPushButton::clicked, [&]() { pages.setCurrentIndex(0); });
  QObject::connect(&edit, &QPushButton::clicked, [&]() {
    if (list.currentRow() >= 0) gameForm(list.currentRow());
  });
  QObject::connect(&list, &QListWidget::itemDoubleClicked, [&]() {
    gameForm(list.currentRow());
  });
  QObject::connect(&remove, &QPushButton::clicked, [&]() {
    int index = list.currentRow();
    if (index < 0) return;
    if (QMessageBox::question(&window, "Delete Game", "Hapus game yang dipilih?") == QMessageBox::Yes) {
      library.deleteGame(index);
      refresh();
    }
  });
  QObject::connect(&exit, &QPushButton::clicked, &window, &QWidget::close);
  window.show();
  return app.exec();
}
