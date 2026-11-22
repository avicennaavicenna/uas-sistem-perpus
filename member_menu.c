#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "member_menu.h"
#include "circulation.h"

void memberMenu(
    BookList *books,
    TransactionList *transactions,
    const char *username
) {

    char input[30];
    int choice = 0;

    do {

        printf("\n");
        printf("========================================\n");
        printf("       SISTEM MANAJEMEN PERPUSTAKAAN\n");
        printf("========================================\n");

        printf("Selamat datang, %s!\n", username);

        printf("\n");
        printf("1. Lihat Katalog Buku\n");
        printf("2. Cari Buku\n");
        printf("3. Pinjam Buku\n");
        printf("4. Kembalikan Buku\n");
        printf("5. Riwayat Peminjaman\n");
        printf("6. Logout\n");

        printf("\nPilih menu: ");

        if (fgets(input, sizeof(input), stdin) == NULL) {
            return;
        }

       char *end;
       long number = strtol(input, &end, 10);

        // Melewati spasi atau tab setelah angka
        while (*end == ' ' || *end == '\t') {
                end++;
        }

        // Memeriksa apakah input berupa angka 1 sampai 6
        if (end == input || *end != '\n' && *end != '\0' ||
            number < 1 || number > 6) {

            printf("\nPilihan tidak valid!\n");
            continue;
        }

        choice = (int)number;

        switch (choice) {

            case 1:
                showAvailableBooks(books);
                break;

            case 2:
                searchBook(books);
                break;

            case 3:
                borrowBook(
                    books,
                    transactions,
                    username
                );
                break;

            case 4:
                returnBook(
                    books,
                    transactions,
                    username
                );
                break;

            case 5:
                showHistory(
                    transactions,
                    username
                );
                break;

            case 6:
                printf("\nBerhasil logout. Terima kasih!\n");
                break;
        }

        if (choice != 6) {

            printf("\nTekan Enter untuk melanjutkan...");
            fgets(input, sizeof(input), stdin);
        }

    } while (choice != 6);
}