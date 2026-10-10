#include "../include/GameLibrary.h"
#include <QApplication>
#include <stdexcept>
#include <QComboBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QFont>
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
  window.setMinimumSize(460, 380);
  window.setStyleSheet(
      "QWidget { background-color: #f5f6f8; color: #263244; }"
      "QPushButton { background-color: white; border: 1px solid #cbd2dc;"
      " border-radius: 6px; padding: 8px 16px; }"
      "QPushButton:hover { background-color: #e8eef7; }"
      "QPushButton:pressed { background-color: #d8e3f3; }"
      "QPushButton:disabled { color: #929aa5; background-color: #eceef1; }"
      "QPushButton#add { background-color: #386bc0; color: white; border-color: #386bc0; }"
      "QPushButton#add:hover { background-color: #2d58a0; }"
      "QLineEdit, QTextEdit, QListWidget, QComboBox, QSpinBox, QDoubleSpinBox {"
      " background-color: white; border: 1px solid #cbd2dc; border-radius: 4px; padding: 5px; }"
      "QListWidget::item { padding: 7px; }"
      "QListWidget::item:selected { background-color: #dce8fa; color: #263244; }"
  );
  QVBoxLayout layout(&window);
  layout.setContentsMargins(24, 16, 24, 24);
  layout.setSpacing(14);
  QLabel title("Game Library");
  QFont titleFont = title.font();
  titleFont.setPointSize(24);
  titleFont.setBold(true);
  title.setFont(titleFont);
  title.setAlignment(Qt::AlignCenter);
  layout.addSpacing(30);
  layout.addWidget(&title);
  QStackedWidget pages;
  layout.addWidget(&pages);

  QWidget home;
  QVBoxLayout menu(&home);
  menu.setContentsMargins(65, 0, 65, 0);
  menu.setSpacing(12);
  QPushButton add("Add Game"), show("Show Games"), exit("Keluar");
  add.setObjectName("add");
  add.setMinimumHeight(40);
  show.setMinimumHeight(40);
  exit.setMinimumHeight(40);
  menu.addStretch();
  menu.addWidget(&add);
  menu.addWidget(&show);
  menu.addWidget(&exit);
  menu.addStretch();
  pages.addWidget(&home);

  QWidget collection;
  QVBoxLayout collectionLayout(&collection);
  QLabel hint("Double-click untuk info, atau pilih game lalu klik Edit.");
  hint.setWordWrap(true);
  QListWidget list;
  QPushButton edit("Edit Game"), remove("Delete Game"), back("Kembali");
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
    hint.setText(list.count() ? "Double-click untuk info, atau pilih game lalu klik Edit."
                             : "Belum ada game. Tambahkan lewat menu utama.");
  };

  auto gameForm = [&](int index) {
    const GAME *game = library.getGame(index);
    QDialog dialog(&window);
    dialog.setWindowTitle(game ? "Detail / Edit Game" : "Add Game");
    dialog.resize(400, 300);
    QFormLayout form(&dialog);
    form.setContentsMargins(20, 20, 20, 20);
    form.setSpacing(12);
    QLineEdit name;
    name.setPlaceholderText("Nama game");
    QComboBox platform;
    platform.addItems({"PC", "Mobile", "Console"});
    QLineEdit launchTarget;
    QLabel launchLabel;
    auto updateLaunchField = [&]() {
      QStringList labels = {"Launcher PC", "OS Mobile", "Nama Konsol"};
      QStringList defaults = {"Steam", "Android", "PlayStation 5"};
      launchLabel.setText(labels[platform.currentIndex()]);
      if (game && platform.currentText() == QString::fromStdString(game->getPlatform()))
        launchTarget.setText(QString::fromStdString(game->getLaunchTarget()));
      else launchTarget.setText(defaults[platform.currentIndex()]);
    };
    QObject::connect(&platform, &QComboBox::currentIndexChanged, [&]() { updateLaunchField(); });
    updateLaunchField();
    QDoubleSpinBox price;
    price.setRange(0, 1000000000);
    price.setPrefix("Rp ");
    QSpinBox rating;
    rating.setRange(0, 10);
    rating.setSuffix(" / 10");
    QTextEdit description;
    description.setPlaceholderText("Catatan tentang game...");
    description.setMaximumHeight(80);
    form.addRow("Nama", &name);
    form.addRow("Platform", &platform);
    form.addRow(&launchLabel, &launchTarget);
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
    achievement.setPlaceholderText("Achievement baru");
    QPushButton addAchievement("Tambah");
    QHBoxLayout achievementRow;
    achievementRow.addWidget(&achievement);
    achievementRow.addWidget(&addAchievement);
    PCGAME pending;

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
      float hours = static_cast<float>(extraTime.value());
      pending.calcPlayTime(hours);
      totalTime.setText(QString::number(game->getPlayTime() + pending.getPlayTime()) + " jam");
      extraTime.setValue(0);
    });
    QObject::connect(&addAchievement, &QPushButton::clicked, [&]() {
      QString text = achievement.text().trimmed();
      auto value = text.toStdString();
      try {
        pending.addAchievement(value);
      } catch (const std::invalid_argument &error) {
        QMessageBox::information(&dialog, "Achievement", error.what());
        return;
      }
      achievements.addItem(text);
      achievement.clear();
    });

    QDialogButtonBox buttons(QDialogButtonBox::Save | QDialogButtonBox::Cancel);
    form.addRow(&buttons);
    QObject::connect(&buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    QObject::connect(&buttons, &QDialogButtonBox::accepted, [&]() {
      try {
        auto gameName = name.text().trimmed().toStdString();
        auto selectedPlatform = static_cast<launcher>(platform.currentIndex());
        // Validasi melalui setter sebelum menyimpan perubahan.
        PCGAME details;
        details.setName(gameName);
        details.setLaunchTarget(launchTarget.text().trimmed().toStdString());
        float cost = static_cast<float>(price.value());
        int score = rating.value();
        details.setPrice(cost);
        details.setRating(score);
        if (game) library.editGame(index, gameName, selectedPlatform, launchTarget.text().trimmed().toStdString());
        else {
          index = static_cast<int>(library.showGames().size());
          library.addGame(gameName, selectedPlatform, launchTarget.text().trimmed().toStdString());
        }
        library.updateDetails(index, cost, score, description.toPlainText().toStdString());
        library.calcPlayTime(index, pending.getPlayTime());
        for (const auto &item : pending.getAchievements()) library.addAchievement(index, item);
        dialog.accept();
      } catch (const std::invalid_argument &error) {
        QMessageBox::information(&dialog, "Data tidak valid", error.what());
      }
    });
    if (dialog.exec() != QDialog::Accepted) return;
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
    const GAME *game = library.getGame(list.currentRow());
    if (!game) return;
    QDialog dialog(&window);
    dialog.setWindowTitle("Info Game");
    dialog.resize(400, 350);
    QFormLayout form(&dialog);
    form.setContentsMargins(20, 20, 20, 20);
    form.setSpacing(12);
    QLabel name(QString::fromStdString(game->getName()));
    QLabel platform(QString::fromStdString(game->getPlatform()));
    QLabel price(game->getPrice() == 0
                     ? "Gratis"
                     : QString("Rp %1").arg(game->getPrice(), 0, 'f', 2));
    QLabel rating(QString("%1 / 10").arg(game->getRating()));
    QLabel time(QString("%1 jam").arg(game->getPlayTime()));
    QTextEdit description;
    description.setReadOnly(true);
    description.setPlainText(QString::fromStdString(game->getDescription()));
    description.setMaximumHeight(90);
    QListWidget achievements;
    for (const auto &item : game->getAchievements())
      achievements.addItem(QString::fromStdString(item));
    if (achievements.count() == 0) achievements.addItem("Belum ada achievement.");
    form.addRow("Nama", &name);
    form.addRow("Platform", &platform);
    form.addRow("Harga", &price);
    form.addRow("Rating", &rating);
    form.addRow("Waktu bermain", &time);
    QLabel instructions(QString::fromStdString(game->getLaunchInstructions()));
    instructions.setWordWrap(true);
    form.addRow("Cara menjalankan", &instructions);
    form.addRow("Deskripsi", &description);
    form.addRow("Achievement", &achievements);
    QDialogButtonBox buttons(QDialogButtonBox::Close);
    form.addRow(&buttons);
    QObject::connect(&buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    dialog.exec();
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
