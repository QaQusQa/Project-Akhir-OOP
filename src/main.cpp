#include "../include/GameLibrary.h"
#include "../include/Manager.h"
#include <QFile>
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

  QFile styleFile(":/style.qss");
  if (styleFile.open(QFile::ReadOnly | QFile::Text)) {
    app.setStyleSheet(QString::fromUtf8(styleFile.readAll()));
    styleFile.close();
  } 

  GameLibrary library;
  QWidget window;
  app.setApplicationName("Uap");
  window.setWindowTitle("Uap — Koleksi Game");
  window.resize(1000, 680);
  window.setMinimumSize(760, 540);

  QVBoxLayout layout(&window);
  layout.setContentsMargins(24, 20, 24, 20);
  layout.setSpacing(20);
  QHBoxLayout header;
  QLabel title("◉  UAP");
  title.setObjectName("title");
  QFont titleFont = title.font();
  titleFont.setPointSize(24);
  titleFont.setBold(true);
  title.setFont(titleFont);
  header.addWidget(&title);
  header.addSpacing(28);
  QPushButton homeNav("BERANDA"), libraryNav("KOLEKSI");
  homeNav.setObjectName("navigation");
  libraryNav.setObjectName("navigation");
  homeNav.setCheckable(true);
  libraryNav.setCheckable(true);
  homeNav.setChecked(true);
  header.addWidget(&homeNav);
  header.addWidget(&libraryNav);
  header.addStretch();
  layout.addLayout(&header);
  QStackedWidget pages;
  layout.addWidget(&pages);

  QWidget home;
  QVBoxLayout menu(&home);
  home.setObjectName("hero");
  menu.setContentsMargins(40, 32, 40, 32);
  menu.setSpacing(16);
  QLabel welcomeTag("SELAMAT DATANG DI UAP");
  welcomeTag.setObjectName("eyebrow");
  QLabel welcome("Semua game.\nSatu tempat.");
  welcome.setObjectName("heroTitle");
  QLabel intro("Kumpulkan game favoritmu, catat waktu bermain,\ndan simpan setiap pencapaian dalam koleksimu.");
  intro.setWordWrap(true);
  intro.setObjectName("heroDescription");
  QLabel collectionCount;
  collectionCount.setObjectName("stats");
  QPushButton add("+  Tambah Game"), show("Buka Koleksi  →"), exit("Keluar dari Uap");
  add.setObjectName("add");
  add.setMinimumHeight(40);
  show.setMinimumHeight(40);
  exit.setMinimumHeight(40);
  menu.addStretch();
  menu.addWidget(&welcomeTag);
  menu.addWidget(&welcome);
  menu.addWidget(&intro);
  menu.addWidget(&collectionCount);
  QHBoxLayout heroActions;
  heroActions.addWidget(&show);
  heroActions.addWidget(&add);
  heroActions.addStretch();
  menu.addLayout(&heroActions);
  menu.addStretch();
  pages.addWidget(&home);

  QWidget collection;
  QVBoxLayout collectionLayout(&collection);
  collectionLayout.setContentsMargins(0, 0, 0, 0);
  QLabel collectionTitle("KOLEKSI GAME");
  collectionTitle.setObjectName("sectionTitle");
  QLabel hint("Double-click untuk info, atau pilih game lalu klik Edit.");
  hint.setWordWrap(true);
  QLineEdit search;
  search.setPlaceholderText("Cari nama game...");
  search.setClearButtonEnabled(true);
  QListWidget list;
  list.setSpacing(4);
  QPushButton edit("Edit Game"), remove("Hapus Game"), back("← Beranda"), collectionAdd("+  Tambah Game");
  collectionAdd.setObjectName("add");
  remove.setObjectName("danger");
  QHBoxLayout actions;
  actions.addWidget(&edit);
  actions.addWidget(&remove);
  actions.addStretch();
  actions.addWidget(&collectionAdd);
  collectionLayout.addWidget(&collectionTitle);
  collectionLayout.addWidget(&hint);
  collectionLayout.addWidget(&search);
  collectionLayout.addWidget(&list);
  collectionLayout.addLayout(&actions);
  collectionLayout.addWidget(&back);
  pages.addWidget(&collection);
  QHBoxLayout footer;
  QLabel footerBrand("UAP  /  Koleksi pribadi");
  footerBrand.setObjectName("eyebrow");
  footer.addWidget(&footerBrand);
  footer.addStretch();
  footer.addWidget(&exit);
  layout.addLayout(&footer);

  auto selectedGameIndex = [&]() {
    const auto *item = list.currentItem();
    return item ? item->data(Qt::UserRole).toInt() : -1;
  };

  auto refresh = [&]() {
    int selectedIndex = selectedGameIndex();
    const QString query = search.text().trimmed();
    const auto games = library.showGames();
    collectionCount.setText(QString::number(games.size()) + " GAME DALAM KOLEKSIMU");
    list.clear();
    for (int index = 0; index < static_cast<int>(games.size()); ++index) {
      const GAME *game = library.getGame(index);
      if (!QString::fromStdString(game->getName()).contains(query, Qt::CaseInsensitive))
        continue;
      auto *item = new QListWidgetItem(QString::fromStdString(games[index]), &list);
      item->setData(Qt::UserRole, index);
      if (index == selectedIndex) list.setCurrentItem(item);
    }
    if (list.count() > 0 && !list.currentItem()) list.setCurrentRow(0);
    edit.setEnabled(list.count() > 0);
    remove.setEnabled(list.count() > 0);
    collectionTitle.setText(QString("KOLEKSI GAME  /  %1 dari %2").arg(list.count()).arg(games.size()));
    hint.setText(list.count() ? "Double-click untuk info, atau pilih game lalu klik Edit."
                             : games.empty() ? "Belum ada game. Tambahkan lewat menu utama."
                                             : "Tidak ada game yang cocok dengan pencarian.");
  };
  QObject::connect(&search, &QLineEdit::textChanged, [&]() { refresh(); });

  auto gameForm = [&](int index) {
    const GAME *game = library.getGame(index);
    QDialog dialog(&window);
    dialog.setWindowTitle(game ? "Uap — Edit Game" : "Uap — Tambah Game");
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

      if (!MANAGER::verifyPlayTime(hours)) {
        QMessageBox::information(&dialog, "Waktu",
                                 "Waktu bermain tidak boleh kosong");
        return;
      }
      pending.calcPlayTime(hours);
      totalTime.setText(QString::number(game->getPlayTime() + pending.getPlayTime()) + " jam");
      extraTime.setValue(0);
    });
    QObject::connect(&addAchievement, &QPushButton::clicked, [&]() {
      QString text = achievement.text().trimmed();
      auto value = text.toStdString();
      
        if (!MANAGER::verifyAchievement(value)) {
            QMessageBox::information(&dialog, "Achievement", "Achievement tidak boleh kosong");
            return;
        }
      
      pending.addAchievement(value);
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
        float cost = static_cast<float>(price.value());
        int score = rating.value();
        auto target = launchTarget.text().trimmed().toStdString();
       
        if (!MANAGER::verifyName(gameName)) {
            throw std::invalid_argument("Nama game harus diisi");
        }
        if (!MANAGER::verifyLaunchTarget(target)) {
            throw std::invalid_argument("Target launcher/OS/konsol harus diisi");
        }
        if (!MANAGER::verifyPrice(cost)) {
          throw std::invalid_argument("Harga harus nol atau lebih");
        }
        if (!MANAGER::verifyRating(score)) {
          throw std::invalid_argument("Rating harus antara 0 sampai 10");
        }

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
  QObject::connect(&collectionAdd, &QPushButton::clicked, [&]() { gameForm(-1); });
  auto openCollection = [&]() {
    refresh();
    pages.setCurrentIndex(1);
    homeNav.setChecked(false);
    libraryNav.setChecked(true);
  };
  auto openHome = [&]() {
    pages.setCurrentIndex(0);
    homeNav.setChecked(true);
    libraryNav.setChecked(false);
  };
  QObject::connect(&show, &QPushButton::clicked, openCollection);
  QObject::connect(&libraryNav, &QPushButton::clicked, openCollection);
  QObject::connect(&homeNav, &QPushButton::clicked, openHome);
  QObject::connect(&back, &QPushButton::clicked, openHome);
  QObject::connect(&edit, &QPushButton::clicked, [&]() {
    if (selectedGameIndex() >= 0) gameForm(selectedGameIndex());
  });
  QObject::connect(&list, &QListWidget::itemDoubleClicked, [&]() {
    const GAME *game = library.getGame(selectedGameIndex());
    if (!game) return;
    QDialog dialog(&window);
    dialog.setWindowTitle("Uap — Info Game");
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
    int index = selectedGameIndex();
    if (index < 0) return;
    if (QMessageBox::question(&window, "Uap — Hapus Game", "Hapus game yang dipilih?") == QMessageBox::Yes) {
      library.deleteGame(index);
      refresh();
    }
  });
  QObject::connect(&exit, &QPushButton::clicked, &window, &QWidget::close);
  refresh();
  window.show();
  return app.exec();
}
