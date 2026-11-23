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