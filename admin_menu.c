#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>

#include "admin_menu.h"
#include "catalog.h"
#include "database.h"

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
    fgets(input, sizeof(input), stdin);
}

/* Mencari judul buku berdasarkan ID buku. */
static const char *findBookTitle(const char *ocaid)
{
    int i;

    for (i = 0; i < bookCount; i++) {
        if (strcmp(books[i].ocaid, ocaid) == 0) {
            return books[i].title;
        }
    }

    return "Buku tidak ditemukan";
}
void showTransactionReport(void)
{
    int i;

    printf("\n");
    printf("============================================================\n");
    printf("              LAPORAN TRANSAKSI PERPUSTAKAAN\n");
    printf("============================================================\n");

    if (transactionCount == 0) {
        printf("Belum ada transaksi peminjaman.\n");
        return;
    }


    for (i = 0; i < transactionCount; i++) {
        printf("\nTransaksi ke-%d\n", i + 1);
        printf("--------------------------------------------\n");

        printf("ID Transaksi : %u\n",
               transactions[i].id_peminjaman);

        printf("Username     : %s\n",
               transactions[i].username);

        printf("ID Buku      : %s\n",
               transactions[i].ocaid);

        printf("Judul Buku   : %s\n",
               findBookTitle(transactions[i].ocaid));

        printf("Tanggal      : %s\n",
               transactions[i].tanggal_pinjam);

        printf("Status       : %s\n",
               transactions[i].status);
    }

    printf("\n============================================================\n");
    printf("Total transaksi: %d\n", transactionCount);
    printf("============================================================\n");
}
void adminMenu(void)
{
    int choice;

    do {
        printf("\n");
        printf("========================================\n");
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
            case 1:
                showCatalog();
                pauseScreen();
                break;

            case 2:
                addBook();
                pauseScreen();
                break;

            case 3:
                editBook();
                pauseScreen();
                break;

            case 4:
                deleteBook();
                pauseScreen();
                break;

            case 5:
                showTransactionReport();
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
