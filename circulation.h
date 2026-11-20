#ifndef CIRCULATION_H
#define CIRCULATION_H

#include "data_structure.h"

// Menampilkan buku yang tersedia
void showAvailableBooks(const BookList *books);

// Mencari buku berdasarkan ID atau judul
void searchBook(const BookList *books);

// Melakukan peminjaman buku
void borrowBook(
    BookList *books,
    TransactionList *transactions,
    const char *username
);

// Melakukan pengembalian buku
void returnBook(
    BookList *books,
    TransactionList *transactions,
    const char *username
);

// Menampilkan riwayat peminjaman pribadi
void showHistory(
    const TransactionList *transactions,
    const char *username
);

#endif