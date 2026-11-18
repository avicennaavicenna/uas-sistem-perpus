
#include <stdio.h>
#include <stdlib.h>
#include "display.h"

/* Membersihkan layar terminal */
void clearScreen(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

/* Menampilkan judul aplikasi */
void tampilkanHeader(void) {
    printf("\n");
    printf("============================================\n");
    printf("       SISTEM MANAJEMEN PERPUSTAKAAN\n");
    printf("============================================\n");
}

/* Menunggu pengguna menekan Enter */
void pauseScreen(void) {
    int karakter;

    printf("\nTekan Enter untuk melanjutkan...");

    while ((karakter = getchar()) != '\n' &&
           karakter != EOF) {
    }
}

/* Menampilkan menu awal */
void tampilkanMenuAwal(void) {
    tampilkanHeader();

    printf("\n             MENU UTAMA\n");
    printf("--------------------------------------------\n");
    printf("1. Login\n");
    printf("2. Keluar\n");
    printf("--------------------------------------------\n");
    printf("Pilih menu: ");
}

/* Menampilkan menu admin */
void tampilkanMenuAdmin(void) {
    tampilkanHeader();

    printf("\n             MENU ADMIN\n");
    printf("--------------------------------------------\n");
    printf("1. Lihat katalog buku\n");
    printf("2. Tambah buku\n");
    printf("3. Edit buku\n");
    printf("4. Hapus buku\n");
    printf("5. Lihat transaksi\n");
    printf("6. Urutkan buku\n");
    printf("7. Logout\n");
    printf("--------------------------------------------\n");
    printf("Pilih menu: ");
}

/* Menampilkan menu user */
void tampilkanMenuUser(void) {
    tampilkanHeader();

    printf("\n             MENU USER\n");
    printf("--------------------------------------------\n");
    printf("1. Lihat katalog buku\n");
    printf("2. Cari buku\n");
    printf("3. Pinjam buku\n");
    printf("4. Kembalikan buku\n");
    printf("5. Riwayat peminjaman\n");
    printf("6. Urutkan buku\n");
    printf("7. Logout\n");
    printf("--------------------------------------------\n");
    printf("Pilih menu: ");
}

/* Menampilkan menu sorting */
void tampilkanMenuSorting(void) {
    tampilkanHeader();

    printf("\n          MENU SORTING BUKU\n");
    printf("--------------------------------------------\n");
    printf("1. Urutkan berdasarkan ID\n");
    printf("2. Urutkan berdasarkan Judul\n");
    printf("3. Urutkan berdasarkan Tahun\n");
    printf("4. Urutkan berdasarkan Jumlah Halaman\n");
    printf("5. Urutkan berdasarkan Stok\n");
    printf("6. Kembali\n");
    printf("--------------------------------------------\n");
    printf("Pilih atribut buku: ");
}

/* Menampilkan pilihan algoritma */
void tampilkanMenuAlgoritma(void) {
    tampilkanHeader();

    printf("\n       PILIH ALGORITMA SORTING\n");
    printf("--------------------------------------------\n");
    printf("1. Bubble Sort\n");
    printf("2. Selection Sort\n");
    printf("3. Insertion Sort\n");
    printf("--------------------------------------------\n");
    printf("Pilih algoritma: ");
}

/* Menampilkan pilihan urutan */
void tampilkanMenuUrutan(void) {
    tampilkanHeader();

    printf("\n          PILIH URUTAN SORTING\n");
    printf("--------------------------------------------\n");
    printf("1. Ascending (Kecil ke Besar / A-Z)\n");
    printf("2. Descending (Besar ke Kecil / Z-A)\n");
    printf("--------------------------------------------\n");
    printf("Pilih urutan: ");
}
