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

    if (*end != '\0' || number <= 0 || number > 2147483647L) {
        return 0;
    }

    *value = (int)number;
    return 1;
}

static int readNonNegativeInt(const char *prompt, int *value)
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

    if (*end != '\0' || number < 0 || number > 2147483647L) {
        return 0;
    }

    *value = (int)number;
    return 1;
}

static int confirmAction(const char *prompt)
{
    char answer[16];

    readLine(prompt, answer, sizeof(answer));
    return (answer[0] == 'y' || answer[0] == 'Y');
}

int hasActiveLoan(const TransactionList *transactions, const char *ocaid)
{
    return is_book_currently_borrowed(transactions, ocaid);
}

void showCatalog(const BookList *books)
{
    if (books == NULL) {
        printf("[ERROR] Data buku tidak tersedia.\n");
        return;
    }

    printf("\n===============================================================================\n");
    printf("                             KATALOG PERPUSTAKAAN\n");
    printf("===============================================================================\n");

    if (books->count == 0) {
        printf("Katalog masih kosong.\n");
        return;
    }

    printf("%-15s %-25s %-20s %-6s %-8s %-6s\n",
           "ID BUKU", "JUDUL", "PENGARANG", "TAHUN", "HALAMAN", "STOK");
    printf("-------------------------------------------------------------------------------\n");

    for (size_t i = 0; i < books->count; i++) {
        const Book *b = &books->items[i];
        printf("%-15.15s %-25.25s %-20.20s %-6d %-8d %-6d\n",
               b->ocaid,
               b->title,
               b->author,
               b->year,
               b->page_count,
               b->stock);
        printf("  Penerbit: %s\n", b->publisher);
    }

    printf("-------------------------------------------------------------------------------\n");
    printf("Jumlah koleksi: %lu buku\n", (unsigned long)books->count);
}

void addBook(BookList *books)
{
    Book newBook;

    if (books == NULL) {
        printf("[ERROR] Data buku tidak tersedia.\n");
        return;
    }

    memset(&newBook, 0, sizeof(newBook));

    printf("\n========== TAMBAH BUKU BARU ==========\n");

    readLine("Masukkan ID buku: ", newBook.ocaid, sizeof(newBook.ocaid));

    if (newBook.ocaid[0] == '\0') {
        printf("ID buku tidak boleh kosong.\n");
        return;
    }

    if (find_book_index_by_id(books, newBook.ocaid) != -1) {
        printf("Gagal: ID buku sudah terdaftar.\n");
        return;
    }

    readLine("Masukkan judul buku: ", newBook.title, sizeof(newBook.title));
    readLine("Masukkan nama pengarang: ", newBook.author, sizeof(newBook.author));
    readLine("Masukkan nama penerbit: ", newBook.publisher, sizeof(newBook.publisher));

    if (newBook.title[0] == '\0' ||
        newBook.author[0] == '\0' ||
        newBook.publisher[0] == '\0') {
        printf("Judul, pengarang, dan penerbit wajib diisi.\n");
        return;
    }

    if (!readPositiveInt("Masukkan tahun terbit: ", &newBook.year)) {
        printf("Tahun terbit harus berupa bilangan positif.\n");
        return;
    }

    if (!readPositiveInt("Masukkan jumlah halaman: ", &newBook.page_count)) {
        printf("Jumlah halaman harus berupa bilangan positif.\n");
        return;
    }

    if (!readNonNegativeInt("Masukkan stok awal: ", &newBook.stock)) {
        printf("Stok harus berupa bilangan nol atau positif.\n");
        return;
    }

    if (!add_book(books, newBook)) {
        printf("Gagal menyimpan buku ke database. Penambahan dibatalkan.\n");
        return;
    }

    printf("Buku berhasil ditambahkan dan disimpan.\n");
}

void editBook(BookList *books)
{
    char id[128];
    int choice;
    Book *target;
    Book oldBook;

    if (books == NULL) {
        printf("[ERROR] Data buku tidak tersedia.\n");
        return;
    }

    printf("\n========== UBAH INFORMASI BUKU ==========\n");
    readLine("Masukkan ID buku yang akan diubah: ", id, sizeof(id));

    target = find_book_by_id(books, id);

    if (target == NULL) {
        printf("Buku dengan ID tersebut tidak ditemukan.\n");
        return;
    }

    oldBook = *target;

    printf("\nData buku yang dipilih:\n");
    printf("ID        : %s\n", target->ocaid);
    printf("Judul     : %s\n", target->title);
    printf("Pengarang : %s\n", target->author);
    printf("Penerbit  : %s\n", target->publisher);
    printf("Tahun     : %d\n", target->year);
    printf("Halaman   : %d\n", target->page_count);
    printf("Stok      : %d\n", target->stock);

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
            readLine("Judul baru: ", target->title, sizeof(target->title));
            if (target->title[0] == '\0') {
                printf("Judul tidak boleh kosong.\n");
                *target = oldBook;
                return;
            }
            break;

        case 2:
            readLine("Pengarang baru: ", target->author, sizeof(target->author));
            if (target->author[0] == '\0') {
                printf("Pengarang tidak boleh kosong.\n");
                *target = oldBook;
                return;
            }
            break;

        case 3:
            readLine("Penerbit baru: ", target->publisher, sizeof(target->publisher));
            if (target->publisher[0] == '\0') {
                printf("Penerbit tidak boleh kosong.\n");
                *target = oldBook;
                return;
            }
            break;

        case 4:
            if (!readPositiveInt("Tahun terbit baru: ", &target->year)) {
                printf("Tahun tidak valid.\n");
                *target = oldBook;
                return;
            }
            break;

        case 5:
            if (!readPositiveInt("Jumlah halaman baru: ", &target->page_count)) {
                printf("Jumlah halaman tidak valid.\n");
                *target = oldBook;
                return;
            }
            break;

        case 6:
            if (!readNonNegativeInt("Stok baru: ", &target->stock)) {
                printf("Stok tidak valid.\n");
                *target = oldBook;
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

    if (!save_books(FILE_BOOKS, books)) {
        *target = oldBook;
        printf("Gagal menyimpan perubahan. Data dikembalikan seperti semula.\n");
        return;
    }

    printf("Informasi buku berhasil diperbarui.\n");
}

void deleteBook(BookList *books, const TransactionList *transactions)
{
    char id[128];
    Book *target;

    if (books == NULL) {
        printf("[ERROR] Data buku tidak tersedia.\n");
        return;
    }

    printf("\n========== HAPUS BUKU ==========\n");
    readLine("Masukkan ID buku yang akan dihapus: ", id, sizeof(id));

    target = find_book_by_id(books, id);

    if (target == NULL) {
        printf("Buku tidak ditemukan.\n");
        return;
    }

    if (hasActiveLoan(transactions, id)) {
        printf("Buku tidak dapat dihapus karena masih memiliki peminjaman aktif.\n");
        return;
    }

    printf("Buku yang akan dihapus: %s - %s\n", target->ocaid, target->title);

    if (!confirmAction("Yakin ingin menghapus buku ini? (y/n): ")) {
        printf("Penghapusan dibatalkan.\n");
        return;
    }

    if (!delete_book_by_id(books, id)) {
        printf("Gagal menghapus buku atau menyimpan perubahan.\n");
        return;
    }

    printf("Buku berhasil dihapus dari katalog.\n");
}