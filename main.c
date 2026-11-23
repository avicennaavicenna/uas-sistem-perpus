#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>

#include "data_structure.h"
#include "database.h"
#include "admin_menu.h"
#include "circulation.h"

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
            /* Menghapus sisa karakter pada buffer input. */
        }
    }
}

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

static int isUsernameTaken(const UserList *users, const char *username)
{
    if (users == NULL) {
        return 0;
    }

    for (size_t i = 0; i < users->count; i++) {
        if (strcmp(users->items[i].username, username) == 0) {
            return 1;
        }
    }

    return 0;
}

static const User *authenticateUser(const UserList *users, const char *username, const char *password)
{
    if (users == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < users->count; i++) {
        if (strcmp(users->items[i].username, username) == 0 &&
            strcmp(users->items[i].password, password) == 0) {
            return &users->items[i];
        }
    }

    return NULL;
}

static void ensureDefaultAdmin(UserList *users)
{
    int hasAdmin = 0;
    User defaultAdmin;

    for (size_t i = 0; i < users->count; i++) {
        if (strcmp(users->items[i].role, "admin") == 0) {
            hasAdmin = 1;
            break;
        }
    }

    if (!hasAdmin) {
        memset(&defaultAdmin, 0, sizeof(defaultAdmin));
        strncpy(defaultAdmin.username, "admin", sizeof(defaultAdmin.username) - 1);
        strncpy(defaultAdmin.password, "admin123", sizeof(defaultAdmin.password) - 1);
        strncpy(defaultAdmin.role, "admin", sizeof(defaultAdmin.role) - 1);

        if (add_user(users, defaultAdmin)) {
            printf("[INFO] Akun admin default dibuat (username: admin | password: admin123)\n");
        }
    }
}

static void handleRegister(UserList *users)
{
    User newUser;

    memset(&newUser, 0, sizeof(newUser));

    printf("\n========== REGISTRASI AKUN BARU ==========\n");
    readLine("Masukkan username: ", newUser.username, sizeof(newUser.username));

    if (newUser.username[0] == '\0') {
        printf("Username tidak boleh kosong.\n");
        return;
    }

    if (strpbrk(newUser.username, "|; \t") != NULL) {
        printf("Username tidak boleh mengandung spasi atau karakter pemisah ('|', ';').\n");
        return;
    }

    if (isUsernameTaken(users, newUser.username)) {
        printf("Registrasi gagal: Username '%s' sudah digunakan.\n", newUser.username);
        return;
    }

    readLine("Masukkan password: ", newUser.password, sizeof(newUser.password));

    if (newUser.password[0] == '\0') {
        printf("Password tidak boleh kosong.\n");
        return;
    }

    if (strpbrk(newUser.password, "|;") != NULL) {
        printf("Password tidak boleh mengandung karakter pemisah ('|', ';').\n");
        return;
    }

    strncpy(newUser.role, "user", sizeof(newUser.role) - 1);

    if (!add_user(users, newUser)) {
        printf("Gagal menyimpan akun baru ke database.\n");
        return;
    }

    printf("Registrasi berhasil! Silakan login menggunakan akun Anda.\n");
}

static void userMenu(BookList *books, TransactionList *transactions, const char *username)
{
    int choice;

    do {
        printf("\n========================================\n");
        printf("         SISTEM PERPUSTAKAAN\n");
        printf("             MENU PENGGUNA\n");
        printf("========================================\n");
        printf("1. Tampilkan buku yang tersedia\n");
        printf("2. Cari buku (ID atau Judul)\n");
        printf("3. Pinjam buku\n");
        printf("4. Kembalikan buku\n");
        printf("5. Riwayat peminjaman saya\n");
        printf("0. Logout\n");
        printf("========================================\n");

        choice = readMenuChoice();

        switch (choice) {
            case 1:
                showAvailableBooks(books);
                pauseScreen();
                break;

            case 2:
                searchBook(books);
                pauseScreen();
                break;

            case 3:
                borrowBook(books, transactions, username);
                pauseScreen();
                break;

            case 4:
                returnBook(books, transactions, username);
                pauseScreen();
                break;

            case 5:
                showHistory(transactions, username);
                pauseScreen();
                break;

            case 0:
                printf("\nAnda berhasil logout.\n");
                break;

            default:
                printf("\nPilihan tidak valid! Silakan masukkan pilihan yang tersedia.\n");
                pauseScreen();
                break;
        }
    } while (choice != 0);
}

static void handleLogin(BookList *books, const UserList *users, TransactionList *transactions)
{
    char username[64];
    char password[64];
    const User *loggedIn;

    printf("\n================= LOGIN =================\n");
    readLine("Username : ", username, sizeof(username));
    readLine("Password : ", password, sizeof(password));

    loggedIn = authenticateUser(users, username, password);

    if (loggedIn == NULL) {
        printf("\nLogin gagal: Username atau password salah.\n");
        return;
    }

    printf("\nSelamat datang, %s (%s)!\n", loggedIn->username, loggedIn->role);

    if (strcmp(loggedIn->role, "admin") == 0) {
        adminMenu(books, transactions);
    } else {
        userMenu(books, transactions, loggedIn->username);
    }
}

int main(void)
{
    BookList books;
    UserList users;
    TransactionList transactions;
    int choice;

    init_book_list(&books);
    init_user_list(&users);
    init_transaction_list(&transactions);

    if (!load_books(FILE_BOOKS, &books)) {
        printf("[PERINGATAN] File '%s' belum ditemukan. Pastikan nama file dataset sudah sesuai.\n", FILE_BOOKS);
    }

    load_users(FILE_USERS, &users);
    load_transactions(FILE_TRANSACTIONS, &transactions);

    ensureDefaultAdmin(&users);

    do {
        printf("\n========================================\n");
        printf("      SISTEM MANAJEMEN PERPUSTAKAAN\n");
        printf("========================================\n");
        printf("1. Login\n");
        printf("2. Registrasi Akun User\n");
        printf("0. Keluar\n");
        printf("========================================\n");

        choice = readMenuChoice();

        switch (choice) {
            case 1:
                handleLogin(&books, &users, &transactions);
                break;

            case 2:
                handleRegister(&users);
                break;

            case 0:
                printf("\nTerima kasih telah menggunakan Sistem Perpustakaan.\n");
                break;

            default:
                printf("\nPilihan tidak valid! Silakan masukkan 0, 1, atau 2.\n");
                break;
        }
    } while (choice != 0);

    free_book_list(&books);
    free_user_list(&users);
    free_transaction_list(&transactions);

    return 0;
}
