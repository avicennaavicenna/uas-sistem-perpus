
#include <stdio.h>
#include <string.h>
#include "login.h"
#include "database.h"

/* Memeriksa username dan password */
int prosesLogin(UserList *daftarUser, User *userLogin) {
    char username[64];
    char password[64];

    if (daftarUser == NULL || userLogin == NULL) {
        return 0;
    }

    printf("\n========== LOGIN ==========\n");

    printf("Username: ");
    scanf(" %63s", username);

    printf("Password: ");
    scanf(" %63s", password);

    /* Mencari akun berdasarkan username */
    User *akun = find_user_by_username(daftarUser, username);

    if (akun != NULL &&
        strcmp(akun->password, password) == 0) {

        *userLogin = *akun;

        printf("\nLogin berhasil!\n");
        return 1;
    }

    printf("\nUsername atau password salah!\n");
    return 0;
}

/* Menampilkan informasi pengguna */
void tampilkanInfoUser(User user) {
    printf("\n========== INFORMASI AKUN ==========\n");
    printf("Username : %s\n", user.username);
    printf("Role     : %s\n", user.role);
    printf("====================================\n");
}
