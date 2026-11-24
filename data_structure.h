#ifndef DATA_STRUCTURE_H
#define DATA_STRUCTURE_H

#include <stddef.h>

/* Nama berkas database */
#define FILE_BOOKS        "Books.txt"
#define FILE_USERS        "user.txt"
#define FILE_TRANSACTIONS "peminjaman.txt"

/* Nilai bawaan untuk dataset awal yang belum memiliki kolom stok/penerbit */
#define DEFAULT_BOOK_STOCK 5
#define DEFAULT_PUBLISHER  "-"

/* Konstanta status transaksi (sesuai kesepakatan dengan Anggota 3) */
#define STATUS_DIPINJAM "DIPINJAM"
#define STATUS_KEMBALI  "KEMBALI"

/* 1. Struktur Data Buku */
typedef struct {
    char ocaid[128];               /* ID unik buku */
    char title[512];               /* Judul buku */
    char author[512];              /* Pengarang */
    char publisher[256];           /* Penerbit */
    int year;                      /* Tahun terbit */
    int page_count;                /* Jumlah halaman */
    int stock;                     /* Stok buku tersedia */
    char openlibrary_edition[128]; /* Metadata tambahan dari dataset */
    char openlibrary_work[128];    /* Metadata tambahan dari dataset */
    char full_text_url[256];       /* Metadata tambahan dari dataset */
} Book;

/* Wadah array dinamis untuk koleksi buku */
typedef struct {
    Book *items;
    size_t count;
    size_t capacity;
} BookList;

/* 2. Struktur Data Akun (User) */
typedef struct {
    char username[64];
    char password[64];
    char role[16]; /* "admin" atau "user" */
} User;

/* Wadah array dinamis untuk daftar akun */
typedef struct {
    User *items;
    size_t count;
    size_t capacity;
} UserList;

/* 3. Struktur Data Transaksi Peminjaman */
typedef struct {
    int id_transaksi;        /* ID unik transaksi (auto-increment) */
    char username[64];       /* Username peminjam */
    char ocaid[128];         /* ID buku yang dipinjam */
    char tanggal_pinjam[24]; /* Format: DD-MM-YYYY */
    char status[16];         /* "DIPINJAM" atau "KEMBALI" */
} Transaction;

/* Wadah array dinamis untuk riwayat transaksi */
typedef struct {
    Transaction *items;
    size_t count;
    size_t capacity;
} TransactionList;

#endif /* DATA_STRUCTURE_H */
