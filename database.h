#ifndef DATABASE_H
#define DATABASE_H

#include "data_structure.h"

/* --- Manajemen Memori Dinamis --- */
void init_book_list(BookList *list);
void free_book_list(BookList *list);

void init_user_list(UserList *list);
void free_user_list(UserList *list);

void init_transaction_list(TransactionList *list);
void free_transaction_list(TransactionList *list);

/* --- Operasi Database Buku (Books.txt) --- */
int load_books(const char *filename, BookList *list);
int save_books(const char *filename, const BookList *list);
int add_book(BookList *list, Book new_book);
int delete_book_by_id(BookList *list, const char *ocaid);
int find_book_index_by_id(const BookList *list, const char *ocaid);
Book *find_book_by_id(BookList *list, const char *ocaid);

/* --- Operasi Database Akun (user.txt) --- */
int load_users(const char *filename, UserList *list);
int save_users(const char *filename, const UserList *list);
int add_user(UserList *list, User new_user);
User *find_user_by_username(UserList *list, const char *username);

/* --- Operasi Database Transaksi (peminjaman.txt) --- */
int load_transactions(const char *filename, TransactionList *list);
int save_transactions(const char *filename, const TransactionList *list);
int get_next_transaction_id(const TransactionList *list);
int add_transaction(TransactionList *list, Transaction new_trx);
int update_transaction_status(TransactionList *list, int id_transaksi, const char *new_status);
Transaction *find_transaction_by_id(TransactionList *list, int id_transaksi);
int is_book_currently_borrowed(const TransactionList *list, const char *ocaid);

#endif /* DATABASE_H */