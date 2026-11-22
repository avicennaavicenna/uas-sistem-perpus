#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "catalog.h"
#include "database.h"

static void readLine(const char *prompt, char *buffer, size_t size)
{
    int ch;
    size_t len;

    printf("%s", prompt);

    if (fgets(buffer, (int)size, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }

    len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    } else {
        while ((ch = getchar()) != '\n' && ch != EOF) {
            /* Buang sisa input yang terlalu panjang. */
        }
    }
}

static int readPositiveInt(const char *prompt, int *value)
{
    char input[64];
    char *end;
    long number;

    readLine(prompt, input, sizeof(input));

    if (input[0] == '\0') {
        return 0;
    }

    number = strtol(input, &end, 10);

    while (*end != '\0' && isspace((unsigned char)*end)) {
        end++;
    }
    
void editBook(void)
{
    char id[64];
    int index;
    int choice;
    Book oldBook;

    printf("\n========== UBAH INFORMASI BUKU ==========\n");
    readLine("Masukkan ID buku yang akan diubah: ", id, sizeof(id));

    index = findBookIndex(id);

    if (index == -1) {
        printf("Buku dengan ID tersebut tidak ditemukan.\n");
        return;
    }

    oldBook = books[index];

    printf("\nData buku yang dipilih:\n");
    printf("ID        : %s\n", books[index].ocaid);
    printf("Judul     : %s\n", books[index].title);
    printf("Pengarang : %s\n", books[index].author);
    printf("Penerbit  : %s\n", books[index].publisher);
    printf("Tahun     : %d\n", books[index].year);
    printf("Halaman   : %d\n", books[index].page_count);
    printf("Stok      : %d\n", books[index].stock);

    printf("\nPilih data yang ingin diubah:\n");
    printf("1. Judul\n");
    printf("2. Pengarang\n");
    printf("3. Penerbit\n");
    printf("4. Tahun terbit\n");
    printf("5. Jumlah halaman\n");
    printf("6. Stok\n");
    printf("0. Batal\n");

    if (!readNonNegativeInt("Pilihan: ", &choice)) {
        printf("Input pilihan tidak valid.\n");
        return;
    }

    switch (choice) {
        case 1:
            readLine("Judul baru: ", books[index].title, sizeof(books[index].title));
            if (books[index].title[0] == '\0') {
                printf("Judul tidak boleh kosong.\n");
                books[index] = oldBook;
                return;
            }
            break;

        case 2:
            readLine("Pengarang baru: ", books[index].author, sizeof(books[index].author));
            if (books[index].author[0] == '\0') {
                printf("Pengarang tidak boleh kosong.\n");
                books[index] = oldBook;
                return;
            }
            break;

        case 3:
            readLine("Penerbit baru: ", books[index].publisher, sizeof(books[index].publisher));
            if (books[index].publisher[0] == '\0') {
                printf("Penerbit tidak boleh kosong.\n");
                books[index] = oldBook;
                return;
            }
            break;

        case 4:
            if (!readPositiveInt("Tahun terbit baru: ", &books[index].year)) {
                printf("Tahun tidak valid.\n");
                books[index] = oldBook;
                return;
            }
            break;

        case 5:
            if (!readPositiveInt("Jumlah halaman baru: ", &books[index].page_count)) {
                printf("Jumlah halaman tidak valid.\n");
                books[index] = oldBook;
                return;
            }
            break;

        case 6:
            if (!readNonNegativeInt("Stok baru: ", &books[index].stock)) {
                printf("Stok tidak valid.\n");
                books[index] = oldBook;
                return;
            }
            break;
             case 0:
            printf("Perubahan dibatalkan.\n");
            return;

        default:
            printf("Pilihan tidak tersedia.\n");
            return;
    }

    if (!saveBooks()) {
        books[index] = oldBook;
        printf("Gagal menyimpan perubahan. Data dikembalikan seperti semula.\n");
        return;
    }

    printf("Informasi buku berhasil diperbarui.\n");
}

void deleteBook(void)
{
    char id[64];
    int index;
    int i;
    Book deletedBook;

    printf("\n========== HAPUS BUKU ==========\n");
    readLine("Masukkan ID buku yang akan dihapus: ", id, sizeof(id));

    index = findBookIndex(id);

    if (index == -1) {
        printf("Buku tidak ditemukan.\n");
        return;
    }

    if (hasActiveLoan(id)) {
        printf("Buku tidak dapat dihapus karena masih memiliki peminjaman aktif.\n");
        return;
    }

    printf("Buku yang akan dihapus: %s - %s\n",
           books[index].ocaid, books[index].title);

    if (!confirmAction("Yakin ingin menghapus buku ini? (y/n): ")) {
        printf("Penghapusan dibatalkan.\n");
        return;
    }

    deletedBook = books[index];

    for (i = index; i < bookCount - 1; i++) {
        books[i] = books[i + 1];
    }

    bookCount--;

    if (!saveBooks()) {
        for (i = bookCount; i > index; i--) {
            books[i] = books[i - 1];
        }
        books[index] = deletedBook;
        bookCount++;
        printf("Gagal menyimpan perubahan. Penghapusan dibatalkan.\n");
        return;
    }

    printf("Buku berhasil dihapus dari katalog.\n");
}
