# Uap — Koleksi Game

Tugas implementasi pilar OOP, mata kuliah Pemrograman Berorientasi Objek.

## Anggota

- Almas Yafi' (25/565863/TK/63831)
- Qaqa Qushayyi Qatrunnada (25/561242/TK/63401)

## Deskripsi

Uap adalah program C++ untuk mengelola koleksi game pribadi dengan tampilan GUI Qt. Nama Uap diambil dari bahasa Indonesia untuk *steam*, dengan tema biru gelap yang terinspirasi dari Steam.

Ada tiga jenis game: PC, Mobile, dan Console. Masing-masing memiliki informasi platform dan petunjuk menjalankan game yang berbeda. Pengguna dapat mencatat harga, rating, deskripsi, waktu bermain, serta achievement. Waktu bermain dan achievement dimasukkan secara manual. Petunjuk menjalankan game ditampilkan sebagai teks.

Data koleksi disimpan ke file JSON dan dimuat kembali saat aplikasi dibuka.

## Fitur

- Beranda dengan jumlah game dalam koleksi.
- Menampilkan koleksi game dari tiga platform.
- Mencari nama game secara langsung tanpa membedakan huruf besar dan kecil.
- Menambah, mengedit, dan menghapus game.
- Mengatur launcher PC, OS Mobile, atau nama konsol.
- Mencatat harga dalam rupiah, rating 0–10, dan deskripsi.
- Menambahkan waktu bermain dan achievement melalui form Edit Game.
- Melihat detail dan petunjuk menjalankan game dengan klik dua kali pada daftar.
- Menyimpan perubahan koleksi secara otomatis ke `games.json`.

## Cara menjalankan

### Kebutuhan

- Compiler yang mendukung C++17.
- CMake versi 3.16 atau lebih baru.
- Qt 6 dengan modul Widgets.

### Build dan menjalankan di Linux

Jalankan perintah berikut dari folder utama proyek:

```bash
cmake -S . -B build
cmake --build build
./build/project-akhir
```

Jalankan aplikasi dari folder utama proyek agar `style.qss` dan `games.json` menggunakan lokasi yang sama. Tema dibaca dari `style.qss` pada folder kerja, dengan alternatif `../style.qss`. Data `games.json` juga menggunakan lokasi relatif terhadap folder kerja.

Jika Qt belum ditemukan oleh CMake, arahkan `CMAKE_PREFIX_PATH` ke lokasi instalasi Qt 6.

## Data contoh

Koleksi tidak diisi otomatis dengan data contoh. Jika `games.json` belum tersedia, aplikasi dimulai dengan koleksi kosong. File dibuat saat koleksi disimpan dan diabaikan oleh Git melalui `.gitignore`.

Contoh format satu game dalam `games.json`:

```json
[
  {
    "name": "Stardew Valley",
    "platform": "PC",
    "launch_target": "Steam",
    "price": 115000,
    "rating": 9,
    "description": "Game simulasi pertanian.",
    "play_time": 12.5,
    "achievements": ["Memanen tanaman pertama"]
  }
]
```

## Struktur file

| File | Isi |
| --- | --- |
| `include/Game.h`, `cpp/Game.cpp` | Class abstrak `GAME` dan tiga class turunannya. |
| `include/GameLibrary.h`, `cpp/GameLibrary.cpp` | Pengelolaan koleksi dan pembuatan objek berdasarkan platform. |
| `include/Storage.h`, `cpp/Storage.cpp` | Penyimpanan dan pembacaan data JSON menggunakan Qt. |
| `include/Manager.h`, `cpp/Manager.cpp` | Fungsi validasi statis untuk input pengguna. |
| `src/main.cpp` | Program utama, GUI, navigasi, pencarian, dan penanganan aksi pengguna. |
| `style.qss` | Tema dan gaya komponen GUI. |
| `CMakeLists.txt` | Pengaturan build C++17 dan Qt 6 Widgets. |
| `games.json` | Data koleksi lokal yang dibuat saat penyimpanan. |

## Class

| Class | Keterangan | Data khusus / nilai awal |
| --- | --- | --- |
| `GAME` | Class abstrak, induk semua game. | Nama, harga, rating, deskripsi, waktu bermain, achievement. |
| `PCGAME` | Game untuk platform PC. | `pcLauncher`, awalnya `Steam`. |
| `MobileGAME` | Game untuk perangkat mobile. | `operatingSystem`, awalnya `Android`. |
| `ConsoleGAME` | Game untuk konsol. | `consoleName`, awalnya `PlayStation 5`. |
| `GameLibrary` | Mengelola tambah, edit, hapus, dan akses game. | `vector<unique_ptr<GAME>>` dan objek `STORAGE`. |
| `STORAGE` | Mengubah data objek menjadi JSON dan membentuk objek kembali saat dibaca. | `filePath`, awalnya `games.json`. |
| `MANAGER` | Memvalidasi nama, harga, rating, waktu bermain, target peluncuran, dan achievement. | Method statis yang mengembalikan `bool`. |

`GameLibrary` memiliki objek-objek game melalui `unique_ptr`, sehingga memori dilepas otomatis saat objek dihapus atau library dihancurkan. Hubungan ini merupakan composition. `STORAGE` juga menjadi bagian dari `GameLibrary`.

Alur utama program:

```text
GUI (main.cpp) → GameLibrary → GAME / class turunan
                     ↓
                  STORAGE → games.json
```

## Penerapan OOP

### Class dan object

`GAME`, ketiga turunannya, `GameLibrary`, `STORAGE`, dan `MANAGER` merupakan class. Objek dibuat, misalnya, melalui `GameLibrary library;` pada program utama dan `make_unique<PCGAME>()` saat menambahkan game PC.

### Encapsulation

Atribut `name`, `price`, `play_time`, dan `achievements` pada `GAME` bersifat private. Perubahan dilakukan melalui method yang memeriksa input. `description` dan `rating` bersifat protected, sehingga dapat diakses class turunan.

Sebagai contoh, setter menolak rating di luar rentang 0–10:

```cpp
void GAME::setRating(int &rating_given) {
  if (rating_given < 0 || rating_given > 10)
    throw std::invalid_argument("Rating harus antara 0 dan 10.");
  rating = rating_given;
}
```

Daftar `games` pada `GameLibrary` juga private. Method `getGame()` mengembalikan `const GAME*`, sehingga pemanggil hanya dapat membaca objek melalui pointer tersebut. GUI memakai `MANAGER` untuk validasi awal, sedangkan method pada `GAME` tetap memeriksa data sebelum mengubah atribut.

### Abstraction

`GAME` adalah class abstrak karena memiliki pure virtual function:

```cpp
virtual string getPlatform() const = 0;
virtual string getLaunchInstructions() const = 0;
virtual const string &getLaunchTarget() const = 0;
virtual void setLaunchTarget(const string &target) = 0;
```

Class turunan wajib menyediakan implementasinya. GUI cukup memanggil method seperti `library.addGame(...)` dan `library.deleteGame(...)`, sementara pembuatan objek dan penyimpanan data ditangani di dalam `GameLibrary`.

### Inheritance

`PCGAME`, `MobileGAME`, dan `ConsoleGAME` mewarisi `GAME` dengan public inheritance. Ketiganya menggunakan atribut umum melalui getter dan setter yang diwarisi, lalu menambahkan data khusus platform.

```cpp
class PCGAME : public GAME {
  string pcLauncher = "Steam";
  // Implementasi fungsi virtual khusus PC.
};
```

### Polymorphism

Ketiga jenis game disimpan dalam satu `vector<unique_ptr<GAME>>`. Method `getPlatform()`, `getLaunchInstructions()`, `getLaunchTarget()`, dan `setLaunchTarget()` di-override oleh setiap class turunan.

Saat GUI memanggil method melalui pointer induk, implementasi dipilih sesuai jenis objek sebenarnya:

```cpp
const GAME *game = library.getGame(selectedGameIndex());
if (game) {
  QLabel instructions(QString::fromStdString(game->getLaunchInstructions()));
}
```

Game PC menampilkan petunjuk membuka launcher, Mobile menampilkan petunjuk mengetuk ikon aplikasi, dan Console menampilkan petunjuk menjalankan game melalui konsol. Destructor virtual pada `GAME` memastikan objek turunan dihancurkan dengan benar melalui pointer induk.

## Skenario demo (GUI)

Gunakan data baru berikut agar demo tidak bergantung pada isi koleksi sebelumnya.

| No. | Aksi | Input | Hasil | Konsep |
| --- | --- | --- | --- | --- |
| 1 | Tambah Game | `Stardew Valley`, PC, launcher `Steam` | Game PC masuk koleksi. | Class dan object, inheritance |
| 2 | Tambah Game | `Mobile Legends`, Mobile, OS `Android` | Game Mobile masuk koleksi. | Inheritance |
| 3 | Tambah Game | `Gran Turismo 7`, Console, konsol `PlayStation 5` | Game Console masuk koleksi. | Inheritance |
| 4 | Klik dua kali setiap game | Ketiga game di atas | Petunjuk menjalankan berbeda sesuai platform. | Polymorphism |
| 5 | Cari nama game | `STARDEW` | Daftar hanya menampilkan nama yang cocok meski kapitalisasi berbeda. | Fitur pencarian GUI |
| 6 | Tambah Game | Nama hanya berisi spasi | Penyimpanan ditolak dengan pesan validasi. | Encapsulation |
| 7 | Edit Game | Harga `115000`, rating `9`, deskripsi bebas | Detail game diperbarui. | Encapsulation, abstraction |
| 8 | Edit Game → Tambah waktu → Simpan | `2.5` jam | Total waktu bertambah 2.5 jam. | Encapsulation |
| 9 | Edit Game → Achievement baru → Tambah → Simpan | `Panen pertama` | Achievement tampil di detail game. | Encapsulation |
| 10 | Hapus Game | Pilih satu game, lalu konfirmasi | Game dihapus dari koleksi dan file JSON. | Abstraction, composition |
| 11 | Tutup dan buka kembali aplikasi | Jalankan dari folder yang sama | Data koleksi dimuat kembali. | Penyimpanan data |

Form membatasi harga dan waktu bermain agar tidak negatif serta rating pada rentang 0–10. Tombol Batal pada form membatalkan perubahan yang belum disimpan.
