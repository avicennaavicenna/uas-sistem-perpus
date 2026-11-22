#ifndef LOGIN_H
#define LOGIN_H

#include "data_structure.h"

/* Memeriksa username dan password */
int prosesLogin(UserList *daftarUser, User *userLogin);

/* Menampilkan informasi pengguna */
void tampilkanInfoUser(User user);

/* Menutup/membersihkan sesi login pengguna saat ini */
void prosesLogout(User *userLogin);

#endif
