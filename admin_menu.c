#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>

#include "admin_menu.h"
#include "catalog.h"
#include "database.h"
#include "sorting.h"

static int readMenuChoice(void)
{
    char input[64];
    char *end;
    long choice;

    printf("Masukkan pilihan: ");

    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }

    if (strchr(input, '\n') == NULL) {
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) {
            /* Menghapus sisa input. */
        }
    }

    errno = 0;
    choice = strtol(input, &end, 10);

    if (end == input || errno == ERANGE) {
        return -1;
    }

    while (*end != '\0' && isspace((unsigned char)*end)) {
        end++;
    }

    if (*end != '\0' || choice < INT_MIN || choice > INT_MAX) {
        return -1;
    }

    return (int)choice;
}

static void pauseScreen(void)
{
    char input[8];

    printf("\nTekan Enter untuk melanjutkan...");
    if (fgets(input, sizeof(input), stdin) == NULL) {
    /* abaikan jika EOF atau gagal baca */
}
}

static const char *findBookTitle(const BookList *books, const char *ocaid)
{
    int idx = find_book_index_by_id(books, ocaid);
    if (idx == -1) {
        return "Buku tidak ditemukan";
    }
    return books->items[idx].title;
}

void showTransactionReport(const BookList *books, const TransactionList *transactions)
{
    if (transactions == NULL) {
        printf("[ERROR] Data transaksi tidak tersedia.\n");
        return;
    }

    printf("\n============================================================\n");
    printf("              LAPORAN TRANSAKSI PERPUSTAKAAN\n");
    printf("============================================================\n");

    if (transactions->count == 0) {
        printf("Belum ada transaksi peminjaman.\n");
        return;
    }

    for (size_t i = 0; i < transactions->count; i++) {
        const Transaction *trx = &transactions->items[i];

        printf("\nTransaksi ke-%lu\n", (unsigned long)(i + 1));
        printf("--------------------------------------------\n");
        printf("ID Transaksi : %d\n", trx->id_transaksi);
        printf("Username     : %s\n", trx->username);
        printf("ID Buku      : %s\n", trx->ocaid);
        printf("Judul Buku   : %s\n", findBookTitle(books, trx->ocaid));
        printf("Tanggal      : %s\n", trx->tanggal_pinjam);
        printf("Status       : %s\n", trx->status);
    }

    printf("\n============================================================\n");
    printf("Total transaksi: %lu\n", (unsigned long)transactions->count);
    printf("============================================================\n");
}

void adminMenu(BookList *books, TransactionList *transactions)
{
    int choice;

    do {
        printf("\n========================================\n");
        printf("         SISTEM PERPUSTAKAAN\n");
        printf("              MENU ADMIN\n");
        printf("========================================\n");
        printf("1. Tampilkan katalog buku\n");
        printf("2. Tambah buku\n");
        printf("3. Ubah informasi buku\n");
        printf("4. Hapus buku\n");
        printf("5. Lihat laporan transaksi\n");
        printf("0. Logout\n");
        printf("========================================\n");

        choice = readMenuChoice();

        switch (choice) {
                case 1: {
                BookList tampilan;

                if (salinDaftarBuku(books, &tampilan)) {
                    menuUrutkanBuku(&tampilan);
                    showCatalog(&tampilan);
                    free(tampilan.items);
                } else {
                    showCatalog(books);
                }
                pauseScreen();
                break;
            }

            case 2:
                addBook(books);
                pauseScreen();
                break;

            case 3:
                editBook(books);
                pauseScreen();
                break;

            case 4:
                deleteBook(books, transactions);
                pauseScreen();
                break;

            case 5:
                showTransactionReport(books, transactions);
                pauseScreen();
                break;

            case 0:
                printf("\nAnda berhasil logout dari akun admin.\n");
                break;

            default:
                printf("\nPilihan tidak valid!\n");
                printf("Silakan masukkan pilihan yang tersedia.\n");
                pauseScreen();
                break;
        }

    } while (choice != 0);
}
