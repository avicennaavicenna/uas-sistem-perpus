# Sistem Manajemen Perpustakaan Berbasis CLI

Aplikasi **Sistem Manajemen Perpustakaan** berbasis antarmuka baris perintah (*Command Line Interface* / CLI) menggunakan bahasa **C standar** untuk Project UAS Praktikum Pemrograman Kelas A, Program Studi Informatika, FMIPA Universitas Syiah Kuala (2026).

---

## 1. Anggota Kelompok

| No | Nama Lengkap | NPM |
| :--- | :--- | :--- |
| 1 | Daffa Alghaitsa | 2608107010000 |
| 2 | Nama Anggota 2 | 2608107010000 |
| 3 | Nama Anggota 3 | 2608107010000 |

---

## 2. Arsitektur Aplikasi dan Struktur File

### Penjelasan Singkat Arsitektur
Aplikasi ini dibangun dengan **arsitektur modular** yang memisahkan lapisan struktur data dan pengurusan memori dinamis, lapisan penyimpanan berkas teks (`.txt`), logika perniagaan (katalog buku dan sirkulasi peminjaman), serta antarmuka CLI:
1. **Inisialisasi & Persistensi Data (`data_structure.h`, `database.h`, `database.c`):** Memuat data dari `Books.txt`, `user.txt`, dan `peminjaman.txt` ke dalam *array* dinamis (`BookList`, `UserList`, `TransactionList`) serta menyinkronkan setiap perubahan secara langsung ke berkas `.txt`.
2. **Autentikasi & Navigasi (`main.c`):** Mengendalikan proses pendaftaran (*register*), log masuk (*login*), serta mengarahkan pengguna ke menu **Admin** atau menu **User** mengikut peranan (*role*).
3. **Modul Admin (`admin_menu.h`, `admin_menu.c`, `catalog.h`, `catalog.c`):** Menguruskan operasi CRUD katalog buku (lihat, tambah, ubah, hapus) dan laporan transaksi keseluruhan.
4. **Modul Sirkulasi User (`circulation.h`, `circulation.c`):** Menguruskan paparan buku tersedia, carian buku, peminjaman, pemulangan (kemas kini stok automatik), serta sejarah peminjaman peribadi.

### Struktur File
```text
uas-sistem-perpus/
├── main.c              # Titik masuk utama, autentikasi, & navigasi menu
├── data_structure.h    # Definisi struct (Book, User, Transaction) & list dinamis
├── database.h          # Deklarasi fungsi pengurusan memori & operasi berkas .txt
├── database.c          # Implementasi baca/tulis dan sinkronisasi data .txt
├── catalog.h           # Deklarasi fungsi pengurusan katalog buku (CRUD admin)
├── catalog.c           # Implementasi paparan, tambah, ubah, dan hapus buku
├── admin_menu.h        # Deklarasi menu pentadbir dan laporan transaksi
├── admin_menu.c        # Implementasi navigasi menu admin & laporan global
├── circulation.h       # Deklarasi fungsi sirkulasi peminjaman & pemulangan
├── circulation.c       # Implementasi pinjam, pulang, cari buku, & riwayat user
├── Books.txt           # Pangkalan data koleksi buku
├── user.txt            # Pangkalan data akaun pengguna (admin & user)
├── peminjaman.txt      # Pangkalan data rekod transaksi peminjaman
└── README.md           # Dokumentasi projek
