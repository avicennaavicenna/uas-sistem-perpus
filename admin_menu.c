#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "admin_menu.h"
#include "catalog.h"
#include "database.h"

static void pauseScreen(void)
{
    char enter[8];

    printf("\nTekan Enter untuk melanjutkan...");
    fgets(enter, sizeof(enter), stdin);
}
static int readMenuChoice(void)
{
    char input[32];
    char *end;
    long choice;

    printf("Masukkan pilihan: ");

    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }

    choice = strtol(input, &end, 10);

    while (*end == ' ' || *end == '\t' || *end == '\r' || *end == '\n') {
        end++;
    }

    if (end == input || *end != '\0') {
        return -1;
    }

    return (int)choice;
}

void showTransactionReport(void)
{
    int i;

    printf("\n===============================================================\n");
    printf("                 LAPORAN SELURUH TRANSAKSI\n");
    printf("===============================================================\n");

    if (transactionCount <= 0) {
        printf("Belum terdapat transaksi peminjaman.\n");
        return;
    }

    printf("%-12s %-20s %-18s %-15s %-12s\n",
           "ID TRANSAKSI", "USERNAME", "ID BUKU", "TANGGAL", "STATUS");
    printf("-------------------------------------------------------------------------------\n");

    for (i = 0; i < transactionCount; i++) {
        printf("%-12u %-20.20s %-18.18s %-15.15s %-12.12s\n",
               transactions[i].id_peminjaman,
               transactions[i].username,
               transactions[i].ocaid,
               transactions[i].tanggal_pinjam,
               transactions[i].status);
    }
    printf("-------------------------------------------------------------------------------\n");
    printf("Total transaksi: %d\n", transactionCount);
}
void adminMenu(void)
{
    int choice;

    do {
        printf("\n");
        printf("========================================\n");
        printf("          MENU ADMIN PERPUSTAKAAN\n");
        printf("========================================\n");
        printf("1. Tampilkan katalog buku\n");
        printf("2. Tambah buku\n");
        printf("3. Ubah informasi buku\n");
        printf("4. Hapus buku\n");
        printf("5. Lihat laporan seluruh transaksi\n");
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
                printf("Logout admin berhasil.\n");
                break;

            default:
                printf("Pilihan tidak valid. Silakan coba lagi.\n");
                pauseScreen();
                break;
        }
    } while (choice != 0);
}