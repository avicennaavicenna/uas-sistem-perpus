# Sistem Manajemen Perpustakaan Berbasis CLI

Aplikasi **Sistem Manajemen Perpustakaan** berbasis antarmuka baris perintah (*Command Line Interface* / CLI) yang dikembangkan menggunakan bahasa pemrograman **C standar** untuk Project UAS Praktikum Pemrograman Kelas A, Program Studi Informatika, FMIPA Universitas Syiah Kuala (2026).

---

## 1. Anggota Kelompok

| No | Nama Lengkap | NPM |
| :--- | :--- | :--- |
| 1 | Daffa Alghaitsa | 260810701100066 |
| 2 | Avicenna | 260810701100026 |
| 3 | Ulya Ananda | 260810701100059|
| 4 | kaiyisa Maritsa | 260810701100062|
---

## 2. Arsitektur Aplikasi dan Struktur File

### Penjelasan Singkat Arsitektur
Aplikasi ini menggunakan **arsitektur modular** yang memisahkan lapisan struktur data dan pengelolaan memori dinamis, lapisan penyimpanan file teks (`.txt`), logika bisnis (manajemen katalog buku dan sirkulasi peminjaman), serta antarmuka pengguna berbasis CLI:
1. **Inisialisasi & Persistensi Data (`data_structure.h`, `database.h`, `database.c`):** Memuat data dari `Books.txt`, `user.txt`, dan `peminjaman.txt` ke dalam array dinamis (`BookList`, `UserList`, `TransactionList`) serta menyinkronkan setiap perubahan data secara langsung ke file `.txt` terkait.
2. **Autentikasi & Navigasi Menu (`main.c`):** Menangani proses registrasi akun baru, login pengguna, serta mengarahkan alur program ke menu **Admin** atau menu **User** sesuai hak akses (*role*).
3. **Modul Admin (`admin_menu.h`, `admin_menu.c`, `catalog.h`, `catalog.c`):** Mengelola operasi CRUD katalog buku (tampilkan, tambah, ubah, hapus) serta menampilkan laporan transaksi peminjaman secara keseluruhan.
4. **Modul Sirkulasi User (`circulation.h`, `circulation.c`):** Mengelola fitur melihat buku yang tersedia, mencari buku, meminjam buku, mengembalikan buku (dengan pembaruan stok otomatis), serta melihat riwayat peminjaman pribadi.

### Struktur File
```text
uas-sistem-perpus/
├── Makefile            # Otomasi kompilasi, eksekusi, dan pembersihan file build
├── main.c              # Titik masuk utama program, autentikasi, & navigasi menu
├── data_structure.h    # Definisi struct (Book, User, Transaction) & list dinamis
├── database.h          # Deklarasi fungsi pengelolaan memori & operasi file .txt
├── database.c          # Implementasi baca/tulis dan sinkronisasi data .txt
├── catalog.h           # Deklarasi fungsi manajemen katalog buku (CRUD admin)
├── catalog.c           # Implementasi tampil, tambah, ubah, dan hapus buku
├── admin_menu.h        # Deklarasi antarmuka menu admin dan laporan transaksi
├── admin_menu.c        # Implementasi navigasi menu admin & laporan global
├── circulation.h       # Deklarasi fungsi sirkulasi peminjaman dan pengembalian
├── circulation.c       # Implementasi pinjam, kembali, cari buku, & riwayat user
├── Books.txt           # Database master koleksi buku perpustakaan
├── user.txt            # Database kredensial akun (admin dan user)
├── peminjaman.txt      # Database catatan transaksi peminjaman dan pengembalian
└── README.md           # Dokumentasi proyek
```

---

## 3. Panduan Kompilasi (*Compile*) dan Menjalankan Program (*Run*)

### Prasyarat
* Compiler bahasa C (**GCC** / MinGW pada Windows atau GCC/Clang pada Linux/macOS).
* Utilitas **`make`** (atau **`mingw32-make`** pada Windows MinGW) jika menggunakan `Makefile`.
* File dataset **`Books.txt`** berada di dalam folder yang sama dengan file hasil kompilasi.

### Opsi A: Menggunakan Makefile (Direkomendasikan)
Buka terminal pada direktori proyek, lalu gunakan perintah berikut *(ganti `make` menjadi `mingw32-make` jika menggunakan MinGW di PowerShell/CMD Windows)*:

* **Kompilasi program (*Compile*):**
  ```bash
  make
  ```
* **Kompilasi sekaligus menjalankan program (*Compile & Run*):**
  ```bash
  make run
  ```
* **Menghapus file hasil kompilasi (`.o` dan executable):**
  ```bash
  make clean
  ```
* **Mereset database operasional (`user.txt` dan `peminjaman.txt`) untuk pengujian:**
  ```bash
  make reset-db
  ```

### Opsi B: Kompilasi Manual via GCC (Tanpa Makefile)
1. **Kompilasi (*Compile*):**
   ```bash
   gcc -Wall -Wextra -std=c11 -O2 main.c database.c catalog.c admin_menu.c circulation.c -o perpustakaan
   ```
2. **Menjalankan Program (*Run*):**
   * **Windows (PowerShell / CMD):**
     ```powershell
     .\perpustakaan.exe
     ```
   * **Linux / macOS:**
     ```bash
     ./perpustakaan
     ```

### Akun Admin Bawaan (*Default Admin*)
Apabila `user.txt` belum memiliki akun dengan peran `admin`, sistem otomatis membuat akun administrator bawaan saat pertama kali dijalankan:
* **Username:** `admin`
* **Password:** `admin123`

---

## 4. Panduan Penggunaan Aplikasi

### A. Menu Awal (Autentikasi)
1. Pilih **`1` (Login)** untuk masuk menggunakan akun `admin` atau akun `user` yang sudah terdaftar.
2. Pilih **`2` (Registrasi Akun User)** untuk membuat akun anggota baru (username tidak boleh mengandung spasi, `|`, atau `;`).
3. Pilih **`0` (Keluar)** untuk menutup aplikasi dan membebaskan memori.

### B. Menu Admin (Pustakawan)
1. **Tampilkan katalog buku:** Menampilkan seluruh koleksi buku beserta ID, judul, pengarang, penerbit, tahun, jumlah halaman, dan stok.
2. **Tambah buku:** Memasukkan data buku baru secara berurutan (ID buku unik, judul, pengarang, penerbit, tahun terbit, jumlah halaman, dan stok awal). Hindari karakter pemisah `|` atau `;` pada teks input.
3. **Ubah informasi buku:** Memasukkan ID buku yang ingin diedit, lalu memilih atribut yang akan diperbarui (judul, pengarang, penerbit, tahun, halaman, atau stok).
4. **Hapus buku:** Menghapus buku berdasarkan ID. Sistem akan menolak penghapusan apabila buku masih memiliki status peminjaman aktif.
5. **Lihat laporan transaksi:** Menampilkan seluruh riwayat transaksi peminjaman dan pengembalian dari semua pengguna.
0. **Logout:** Keluar dari sesi admin kembali ke menu awal.

### C. Menu User (Anggota / Mahasiswa)
1. **Tampilkan buku yang tersedia:** Menampilkan daftar koleksi buku yang memiliki stok lebih dari `0`.
2. **Cari buku (ID atau Judul):** Mencari buku tertentu di katalog berdasarkan kode ID buku maupun kata kunci judul.
3. **Pinjam buku:** Memasukkan ID buku yang ingin dipinjam. Stok buku otomatis berkurang `1` dan transaksi tercatat di `peminjaman.txt`.
4. **Kembalikan buku:** Mengembalikan buku yang sedang dipinjam. Stok buku otomatis bertambah `1` dan status transaksi diperbarui menjadi `KEMBALI`.
5. **Riwayat peminjaman saya:** Menampilkan daftar transaksi peminjaman dan pengembalian milik akun yang sedang login.
0. **Logout:** Keluar dari sesi pengguna kembali ke menu awal.
