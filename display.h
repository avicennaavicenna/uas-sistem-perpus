#ifndef DISPLAY_H
#define DISPLAY_H

/* Membersihkan tampilan layar terminal sesuai sistem operasi */
void clearScreen(void);

/* Menjeda layar hingga pengguna menekan tombol Enter */
void pauseScreen(void);

/* Menampilkan header judul aplikasi sistem manajemen perpustakaan */
void tampilkanHeader(void);

/* Menampilkan menu awal untuk login atau keluar dari aplikasi */
void tampilkanMenuAwal(void);

/* Menampilkan daftar menu navigasi khusus peran admin */
void tampilkanMenuAdmin(void);

/* Menampilkan daftar menu navigasi khusus peran user */
void tampilkanMenuUser(void);

/* Menampilkan menu pemilihan atribut untuk pengurutan buku */
void tampilkanMenuSorting(void);

/* Menampilkan menu pemilihan algoritma sorting manual */
void tampilkanMenuAlgoritma(void);

/* Menampilkan menu pemilihan arah urutan data */
void tampilkanMenuUrutan(void);

#endif
