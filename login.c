#include <stdio.h>
#include <string.h>
#include "login.h"

/* Jika database.h milik Anggota 1 sudah menyediakan find_user_by_username,
 * definisikan HAS_DATABASE_FIND_USER saat kompilasi (misal: -DHAS_DATABASE_FIND_USER)
 * dan sertakan "database.h". Jika belum, fungsi lokal di bawah akan dipakai. */
#ifdef HAS_DATABASE_FIND_USER
#include "database.h"
#else
static const User *cariUserLokal(const UserList *daftarUser, const char *username) {
    if (daftarUser == NULL || daftarUser->items == NULL || username == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < daftarUser->count; i++) {
        if (strcmp(daftarUser->items[i].username, username) == 0) {
            return &daftarUser->items[i];
        }
    }

    return NULL;
}
#endif

static void bersihkanSisaBaris(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
    }
}

/* Memeriksa username dan password */
int prosesLogin(UserList *daftarUser, User *userLogin) {
    char username[64];
    char password[64];

    if (daftarUser == NULL || userLogin == NULL) {
        printf("\nData akun belum tersedia.\n");
        return 0;
    }

    printf("\n========== LOGIN ==========\n");

    printf("Username: ");
    if (scanf(" %63s", username) != 1) {
        bersihkanSisaBaris();
        printf("\nInput username tidak valid!\n");
        return 0;
    }

    printf("Password: ");
    if (scanf(" %63s", password) != 1) {
        bersihkanSisaBaris();
        printf("\nInput password tidak valid!\n");
        return 0;
    }

    bersihkanSisaBaris();

#ifdef HAS_DATABASE_FIND_USER
    const User *akun = find_user_by_username(daftarUser, username);
#else
    const User *akun = cariUserLokal(daftarUser, username);
#endif

    if (akun != NULL && strcmp(akun->password, password) == 0) {
        *userLogin = *akun;
        printf("\nLogin berhasil! Selamat datang, %s.\n", userLogin->username);
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

/* Menutup/membersihkan sesi login pengguna saat ini */
void prosesLogout(User *userLogin) {
    if (userLogin == NULL) {
        return;
    }

    if (userLogin->username[0] != '\0') {
        printf("\nAnda (%s) telah berhasil logout.\n", userLogin->username);
    } else {
        printf("\nSesi telah direset.\n");
    }

    memset(userLogin, 0, sizeof(User));
}
