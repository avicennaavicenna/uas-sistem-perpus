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

/* =====================================================
   FUNGSI BANTU: MENGAMBIL TANGGAL HARI INI
   Format: YYYY-MM-DD
   ===================================================== */

static int getTodayDate(char *buffer, size_t size) {
    time_t now = time(NULL);
    struct tm *currentDate = localtime(&now);

    if (currentDate == NULL) {
        return 0;
    }

    return strftime(buffer, size, "%Y-%m-%d", currentDate) > 0;
}

/* =====================================================
   FUNGSI BANTU: MEMERIKSA PEMINJAMAN GANDA
   ===================================================== */

static int alreadyBorrowed(
    const TransactionList *transactions,
    const char *username,
    const char *ocaid
) {
    if (transactions == NULL || username == NULL || ocaid == NULL) {
        return 0;
    }

    for (size_t i = 0; i < transactions->count; i++) {
        const Transaction *trx = &transactions->items[i];

        if (strcmp(trx->username, username) == 0 &&
            strcmp(trx->ocaid, ocaid) == 0 &&
            strcmp(trx->status, STATUS_DIPINJAM) == 0) {
            return 1;
        }
    }

    return 0;
}

/* =====================================================
   3. MEMINJAM BUKU
   ===================================================== */

void borrowBook(
    BookList *books,
    TransactionList *transactions,
    const char *username
) {
    char ocaid[128];
    Transaction newTrx = {0};

    if (books == NULL || transactions == NULL || username == NULL) {
        printf("\n[ERROR] Data peminjaman tidak valid.\n");
        return;
    }

    printf("\n========================================\n");
    printf("             PEMINJAMAN BUKU\n");
    printf("========================================\n");

    showAvailableBooks(books);

    printf("\nMasukkan ID buku yang ingin dipinjam: ");
    readInput(ocaid, sizeof(ocaid));

    if (ocaid[0] == '\0') {
        printf("\nID buku tidak boleh kosong.\n");
        return;
    }

    // Mencari buku berdasarkan ID
    Book *book = find_book_by_id(books, ocaid);

    if (book == NULL) {
        printf("\n[ GAGAL ] Buku tidak ditemukan.\n");
        return;
    }

    // Memeriksa stok buku
    if (book->stock <= 0) {
        printf("\n[ GAGAL ] Stok buku sedang habis.\n");
        return;
    }

    // Memeriksa peminjaman aktif pengguna
    if (alreadyBorrowed(transactions, username, book->ocaid)) {
        printf("\n[ GAGAL ] Kamu masih meminjam buku ini.\n");
        return;
    }

    // Mengambil tanggal peminjaman
    if (!getTodayDate(
            newTrx.tanggal_pinjam,
            sizeof(newTrx.tanggal_pinjam))) {
        printf("\n[ ERROR ] Gagal mengambil tanggal hari ini.\n");
        return;
    }

    // Menyiapkan data transaksi
    newTrx.id_transaksi = get_next_transaction_id(transactions);

    snprintf(
        newTrx.username,
        sizeof(newTrx.username),
        "%s",
        username
    );

    snprintf(
        newTrx.ocaid,
        sizeof(newTrx.ocaid),
        "%s",
        book->ocaid
    );

    snprintf(
        newTrx.status,
        sizeof(newTrx.status),
        "%s",
        STATUS_DIPINJAM
    );

    // Mengurangi stok buku
    book->stock--;

    // Menyimpan perubahan stok
    if (!save_books(FILE_BOOKS, books)) {
        book->stock++;

        printf("\n[ ERROR ] Gagal menyimpan perubahan stok.\n");
        return;
    }

    // Menambahkan transaksi menggunakan fungsi database teman
    if (!add_transaction(transactions, newTrx)) {
        // Mengembalikan stok jika transaksi gagal
        book->stock++;

        if (!save_books(FILE_BOOKS, books)) {
            printf("\n[PERINGATAN] Gagal mengembalikan stok ke file.\n");
        }

        printf("\n[ GAGAL ] Peminjaman tidak berhasil disimpan.\n");
        return;
    }

    // Menampilkan informasi peminjaman berhasil
    printf("\n========================================\n");
    printf("        PEMINJAMAN BERHASIL!\n");
    printf("========================================\n");
    printf("ID Transaksi   : %d\n", newTrx.id_transaksi);
    printf("Username       : %s\n", newTrx.username);
    printf("ID Buku        : %s\n", book->ocaid);
    printf("Judul Buku     : %s\n", book->title);
    printf("Tanggal Pinjam : %s\n", newTrx.tanggal_pinjam);
    printf("Status         : %s\n", newTrx.status);
    printf("Sisa Stok      : %d\n", book->stock);
    printf("========================================\n");
}

// =====================================================
// 4. MELAKUKAN PENGEMBALIAN BUKU
// =====================================================

void returnBook(
    BookList *books,
    TransactionList *transactions,
    const char *username
) {

    printf("\n========================================\n");
    printf("             PENGEMBALIAN BUKU\n");
    printf("========================================\n");

    int found = 0;

    // Menampilkan transaksi aktif milik user
    for (size_t i = 0; i < transactions->count; i++) {

        Transaction *trx = &transactions->items[i];

        if (strcmp(trx->username, username) == 0 &&
            strcmp(trx->status, STATUS_DIPINJAM) == 0) {

            Book *book = find_book_by_id(books, trx->ocaid);

            printf("\nID Transaksi : %d\n", trx->id_transaksi);
            printf("ID Buku      : %s\n", trx->ocaid);

            if (book != NULL) {
                printf("Judul Buku   : %s\n", book->title);
            }

            printf("Tanggal Pinjam: %s\n", trx->tanggal_pinjam);
            printf("Status       : %s\n", trx->status);

            printf("----------------------------------------\n");

            found = 1;
        }
    }

    if (!found) {
        printf("Kamu tidak memiliki buku yang sedang dipinjam.\n");
        return;
    }

    // Meminta ID transaksi
    char input[30];

    printf("\nMasukkan ID transaksi yang ingin dikembalikan: ");
    readInput(input, sizeof(input));

    char *end;
    long transactionID = strtol(input, &end, 10);

    if (input[0] == '\0' || *end != '\0' ||
        transactionID <= 0 || transactionID > 2147483647L) {

        printf("ID transaksi tidak valid.\n");
        return;
    }

    // Mencari transaksi
    Transaction *trx = find_transaction_by_id(
        transactions,
        (int)transactionID
    );

    // Memeriksa transaksi dan pemiliknya
    if (trx == NULL ||
        strcmp(trx->username, username) != 0 ||
        strcmp(trx->status, STATUS_DIPINJAM) != 0) {

        printf("\nTransaksi tidak ditemukan, bukan milikmu, atau sudah dikembalikan.\n");
        return;
    }

    // Mencari buku berdasarkan ID
    Book *book = find_book_by_id(books, trx->ocaid);

    if (book == NULL) {
        printf("Data buku tidak ditemukan.\n");
        return;
    }

    // Memperbarui status transaksi
    if (!update_transaction_status(
            transactions,
            (int)transactionID,
            STATUS_KEMBALI)) {

        printf("Gagal memperbarui status transaksi.\n");
        return;
    }

    // Menambahkan stok buku
    book->stock++;

    // Menyimpan perubahan stok
    if (!save_books(FILE_BOOKS, books)) {

        // Mengembalikan stok ke kondisi sebelumnya
        book->stock--;

        // Memulihkan status transaksi
        update_transaction_status(
            transactions,
            (int)transactionID,
            STATUS_DIPINJAM
        );

        printf("Gagal menyimpan perubahan stok.\n");
        return;
    }

    printf("\n========================================\n");
    printf("       PENGEMBALIAN BERHASIL!\n");
    printf("========================================\n");

    printf("ID Transaksi : %d\n", trx->id_transaksi);
    printf("Judul Buku   : %s\n", book->title);
    printf("Status       : %s\n", STATUS_KEMBALI);
    printf("Sisa Stok    : %d\n", book->stock);
}

// =====================================================
// 5. MELIHAT RIWAYAT PEMINJAMAN PRIBADI
// =====================================================

void showHistory(
    const TransactionList *transactions,
    const char *username
) {

    printf("\n========================================\n");
    printf("          RIWAYAT PEMINJAMAN\n");
    printf("========================================\n");

    int found = 0;

    for (size_t i = 0; i < transactions->count; i++) {

        const Transaction *trx = &transactions->items[i];

        // Hanya menampilkan riwayat user yang login
        if (strcmp(trx->username, username) == 0) {

            printf("\nID Transaksi : %d\n", trx->id_transaksi);
            printf("ID Buku      : %s\n", trx->ocaid);
            printf("Tanggal Pinjam: %s\n", trx->tanggal_pinjam);
            printf("Status       : %s\n", trx->status);

            printf("----------------------------------------\n");

            found = 1;
        }
    }

    if (!found) {
        printf("Belum ada riwayat peminjaman.\n");
    }
}