#ifndef LOGIN_H
#define LOGIN_H

#include "data_structure.h"

/* Menampilkan form login dan memverifikasi akun pengguna */
int prosesLogin(User *currentUser);

/* Membersihkan status sesi login pengguna saat ini */
void prosesLogout(User *currentUser);

#endif
