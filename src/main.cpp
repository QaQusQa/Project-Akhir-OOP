#include <QApplication>
#include <QLabel>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  QLabel window("Project Akhir OOP GUI berhasil!");
  window.resize(800, 600);
  window.setAlignment(Qt::AlignCenter);
  window.show();

  return app.exec();
}