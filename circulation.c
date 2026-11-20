#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

#include "circulation.h"
#include "database.h"

// =====================================================
// FUNGSI BANTU: MEMBACA INPUT TEKS
// =====================================================

static void readInput(char *text, size_t size) {
    if (fgets(text, size, stdin) != NULL) {
        text[strcspn(text, "\n")] = '\0';
    } else {
        text[0] = '\0';
    }
}

// =====================================================
// FUNGSI BANTU: MEMBANDINGKAN TEKS
// Tidak membedakan huruf besar dan kecil
// =====================================================

static int containsIgnoreCase(const char *text, const char *keyword) {

    if (keyword[0] == '\0') {
        return 0;
    }

    size_t textLength = strlen(text);
    size_t keyLength = strlen(keyword);

    if (keyLength > textLength) {
        return 0;
    }

    for (size_t i = 0; i <= textLength - keyLength; i++) {

        int match = 1;

        for (size_t j = 0; j < keyLength; j++) {

            if (tolower((unsigned char)text[i + j]) !=
                tolower((unsigned char)keyword[j])) {

                match = 0;
                break;
            }
        }

        if (match) {
            return 1;
        }
    }

    return 0;
}
// =====================================================
// 1. MENAMPILKAN KATALOG BUKU YANG TERSEDIA
// =====================================================

void showAvailableBooks(const BookList *books) {

    printf("\n========================================\n");
    printf("          KATALOG BUKU TERSEDIA\n");
    printf("========================================\n");

    if (books->count == 0) {
        printf("Belum ada buku dalam database.\n");
        return;
    }

    int found = 0;

    for (size_t i = 0; i < books->count; i++) {

        const Book *b = &books->items[i];

        if (b->stock > 0) {

            printf("\nID Buku       : %s\n", b->ocaid);
            printf("Judul         : %s\n", b->title);
            printf("Pengarang     : %s\n", b->author);
            printf("Penerbit      : %s\n", b->publisher);
            printf("Tahun Terbit  : %d\n", b->year);
            printf("Jumlah Halaman: %d\n", b->page_count);
            printf("Stok          : %d\n", b->stock);

            printf("----------------------------------------\n");

            found = 1;
        }
    }

    if (!found) {
        printf("Tidak ada buku yang tersedia saat ini.\n");
    }
}

// =====================================================
// 2. MENCARI BUKU BERDASARKAN ID ATAU JUDUL
// =====================================================

void searchBook(const BookList *books) {

    char keyword[200];

    printf("\n========================================\n");
    printf("               CARI BUKU\n");
    printf("========================================\n");

    printf("Masukkan ID atau judul buku: ");
    readInput(keyword, sizeof(keyword));

    if (keyword[0] == '\0') {
        printf("Kata kunci tidak boleh kosong.\n");
        return;
    }

    int found = 0;

    for (size_t i = 0; i < books->count; i++) {

        const Book *b = &books->items[i];

        if (strcmp(b->ocaid, keyword) == 0 ||
            containsIgnoreCase(b->title, keyword)) {

            printf("\nBuku ditemukan!\n");
            printf("ID Buku       : %s\n", b->ocaid);
            printf("Judul         : %s\n", b->title);
            printf("Pengarang     : %s\n", b->author);
            printf("Penerbit      : %s\n", b->publisher);
            printf("Tahun Terbit  : %d\n", b->year);
            printf("Jumlah Halaman: %d\n", b->page_count);
            printf("Stok          : %d\n", b->stock);

            printf("----------------------------------------\n");

            found = 1;
        }
    }

    if (!found) {
        printf("\nBuku tidak ditemukan.\n");
    }
}