#include "database.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* =========================================================================
 * BAGIAN 1: MANAJEMEN MEMORI DINAMIS (MALLOC / REALLOC / FREE)
 * ========================================================================= */

void init_book_list(BookList *list) {
    list->count = 0;
    list->capacity = 32;
    list->items = (Book *)malloc(list->capacity * sizeof(Book));
    if (!list->items) {
        fprintf(stderr, "[ERROR] Gagal mengalokasikan memori untuk BookList.\n");
        exit(EXIT_FAILURE);
    }
}

void free_book_list(BookList *list) {
    free(list->items);
    list->items = NULL;
    list->count = 0;
    list->capacity = 0;
}

static void push_book_memory(BookList *list, Book book) {
    if (list->count >= list->capacity) {
        size_t new_cap = list->capacity * 2;
        Book *temp = (Book *)realloc(list->items, new_cap * sizeof(Book));
        if (!temp) {
            fprintf(stderr, "[ERROR] Gagal memperbesar kapasitas BookList.\n");
            free_book_list(list);
            exit(EXIT_FAILURE);
        }
        list->items = temp;
        list->capacity = new_cap;
    }
    list->items[list->count++] = book;
}

void init_user_list(UserList *list) {
    list->count = 0;
    list->capacity = 16;
    list->items = (User *)malloc(list->capacity * sizeof(User));
    if (!list->items) {
        fprintf(stderr, "[ERROR] Gagal mengalokasikan memori untuk UserList.\n");
        exit(EXIT_FAILURE);
    }
}

void free_user_list(UserList *list) {
    free(list->items);
    list->items = NULL;
    list->count = 0;
    list->capacity = 0;
}

static void push_user_memory(UserList *list, User user) {
    if (list->count >= list->capacity) {
        size_t new_cap = list->capacity * 2;
        User *temp = (User *)realloc(list->items, new_cap * sizeof(User));
        if (!temp) {
            fprintf(stderr, "[ERROR] Gagal memperbesar kapasitas UserList.\n");
            free_user_list(list);
            exit(EXIT_FAILURE);
        }
        list->items = temp;
        list->capacity = new_cap;
    }
    list->items[list->count++] = user;
}

void init_transaction_list(TransactionList *list) {
    list->count = 0;
    list->capacity = 32;
    list->items = (Transaction *)malloc(list->capacity * sizeof(Transaction));
    if (!list->items) {
        fprintf(stderr, "[ERROR] Gagal mengalokasikan memori untuk TransactionList.\n");
        exit(EXIT_FAILURE);
    }
}

void free_transaction_list(TransactionList *list) {
    free(list->items);
    list->items = NULL;
    list->count = 0;
    list->capacity = 0;
}

static void push_transaction_memory(TransactionList *list, Transaction trx) {
    if (list->count >= list->capacity) {
        size_t new_cap = list->capacity * 2;
        Transaction *temp = (Transaction *)realloc(list->items, new_cap * sizeof(Transaction));
        if (!temp) {
            fprintf(stderr, "[ERROR] Gagal memperbesar kapasitas TransactionList.\n");
            free_transaction_list(list);
            exit(EXIT_FAILURE);
        }
        list->items = temp;
        list->capacity = new_cap;
    }
    list->items[list->count++] = trx;
}

/* =========================================================================
 * BAGIAN 2: HELPER PARSER & FORMATTER BERKAS TEKS
 * ========================================================================= */

static int is_integer_string(const char *str) {
    if (!str || *str == '\0') return 0;
    const char *p = str;
    if (*p == '-' || *p == '+') p++;
    if (*p == '\0') return 0;
    while (*p) {
        if (!isdigit((unsigned char)*p)) return 0;
        p++;
    }
    return 1;
}

/* Membaca satu kolom (token) dari baris teks, mendukung tanda petik "..." dan "" */
static int extract_token(const char **cursor, char *out, size_t max_len) {
    const char *p = *cursor;
    size_t idx = 0;

    while (*p == ' ' || *p == '\t') {
        p++;
    }

    if (*p == '\0' || *p == '\r' || *p == '\n') {
        out[0] = '\0';
        *cursor = p;
        return 0;
    }

    if (*p == '"') {
        p++; /* Lewati kutip pembuka */
        while (*p != '\0' && *p != '\r' && *p != '\n') {
            if (*p == '"') {
                if (*(p + 1) == '"' && (*(p + 2) == '\0' || isspace((unsigned char)*(p + 2)))) {
                    if (idx + 1 < max_len) out[idx++] = '"';
                    p += 2;
                    break;
                } else if (*(p + 1) == '"') {
                    if (idx + 1 < max_len) out[idx++] = '"';
                    p += 2;
                } else if (*(p + 1) == '\0' || isspace((unsigned char)*(p + 1))) {
                    p++;
                    break;
                } else {
                    if (idx + 1 < max_len) out[idx++] = '"';
                    p++;
                }
            } else {
                if (idx + 1 < max_len) out[idx++] = *p;
                p++;
            }
        }
    } else {
        while (*p != '\0' && !isspace((unsigned char)*p)) {
            if (idx + 1 < max_len) out[idx++] = *p;
            p++;
        }
    }

    out[idx] = '\0';
    *cursor = p;
    return 1;
}

/* Menulis kolom ke file dengan membungkus tanda petik jika mengandung spasi atau kutip */
static void write_escaped_token(FILE *fp, const char *str) {
    if (!str || str[0] == '\0') {
        fprintf(fp, "\"-\"");
        return;
    }

    int need_quotes = 0;
    for (size_t i = 0; str[i] != '\0'; i++) {
        if (isspace((unsigned char)str[i]) || str[i] == '"') {
            need_quotes = 1;
            break;
        }
    }

    if (!need_quotes) {
        fprintf(fp, "%s", str);
    } else {
        fputc('"', fp);
        for (size_t i = 0; str[i] != '\0'; i++) {
            if (str[i] == '"') {
                fprintf(fp, "\"\"");
            } else {
                fputc(str[i], fp);
            }
        }
        fputc('"', fp);
    }
}

/* =========================================================================
 * BAGIAN 3: PENGELOLAAN DATABASE BUKU (Books.txt)
 * ========================================================================= */

int load_books(const char *filename, BookList *list) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "[ERROR] Berkas '%s' tidak dapat dibuka atau tidak ditemukan.\n", filename);
        return 0;
    }

    list->count = 0;
    char line[2048];
    int line_number = 0;

    while (fgets(line, sizeof(line), fp)) {
        line_number++;
        const char *cursor = line;

        char tok1[128], tok2[512], tok3[512], tok4[256];
        char tok5[64], tok6[64], tok7[64], tok8[128], tok9[128], tok10[256];

        if (!extract_token(&cursor, tok1, sizeof(tok1))) continue;

        /* Lewati baris header jika ada */
        if (line_number == 1 && strcmp(tok1, "ocaid") == 0) {
            continue;
        }

        if (!extract_token(&cursor, tok2, sizeof(tok2)) ||
            !extract_token(&cursor, tok3, sizeof(tok3)) ||
            !extract_token(&cursor, tok4, sizeof(tok4)) ||
            !extract_token(&cursor, tok5, sizeof(tok5))) {
            fprintf(stderr, "[WARNING] Baris %d pada '%s' dilewati karena format tidak lengkap.\n",
                    line_number, filename);
            continue;
        }

        Book b;
        memset(&b, 0, sizeof(Book));
        snprintf(b.ocaid, sizeof(b.ocaid), "%s", tok1);
        snprintf(b.title, sizeof(b.title), "%s", tok2);
        snprintf(b.author, sizeof(b.author), "%s", tok3);

        /* Deteksi otomatis:
         * - Jika tok4 berupa angka, berarti format dataset awal (8 kolom: year ada di tok4)
         * - Jika tok4 berupa teks, berarti format tersimpan lengkap (10 kolom: publisher ada di tok4) */
        if (is_integer_string(tok4)) {
            snprintf(b.publisher, sizeof(b.publisher), "%s", DEFAULT_PUBLISHER);
            b.year = atoi(tok4);
            b.page_count = atoi(tok5);
            b.stock = DEFAULT_BOOK_STOCK;

            extract_token(&cursor, tok6, sizeof(tok6));
            extract_token(&cursor, tok7, sizeof(tok7));
            extract_token(&cursor, tok8, sizeof(tok8));

            snprintf(b.openlibrary_edition, sizeof(b.openlibrary_edition), "%s", tok6[0] ? tok6 : "-");
            snprintf(b.openlibrary_work, sizeof(b.openlibrary_work), "%s", tok7[0] ? tok7 : "-");
            snprintf(b.full_text_url, sizeof(b.full_text_url), "%s", tok8[0] ? tok8 : "-");
        } else {
            if (!extract_token(&cursor, tok6, sizeof(tok6)) ||
                !extract_token(&cursor, tok7, sizeof(tok7))) {
                fprintf(stderr, "[WARNING] Baris %d pada '%s' dilewati karena kolom stok hilang.\n",
                        line_number, filename);
                continue;
            }

            snprintf(b.publisher, sizeof(b.publisher), "%s", tok4);
            b.year = atoi(tok5);
            b.page_count = atoi(tok6);
            b.stock = atoi(tok7);

            extract_token(&cursor, tok8, sizeof(tok8));
            extract_token(&cursor, tok9, sizeof(tok9));
            extract_token(&cursor, tok10, sizeof(tok10));

            snprintf(b.openlibrary_edition, sizeof(b.openlibrary_edition), "%s", tok8[0] ? tok8 : "-");
            snprintf(b.openlibrary_work, sizeof(b.openlibrary_work), "%s", tok9[0] ? tok9 : "-");
            snprintf(b.full_text_url, sizeof(b.full_text_url), "%s", tok10[0] ? tok10 : "-");
        }

        push_book_memory(list, b);
    }

    fclose(fp);
    return 1;
}

int save_books(const char *filename, const BookList *list) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        fprintf(stderr, "[ERROR] Gagal menyimpan perubahan ke '%s'.\n", filename);
        return 0;
    }

    fprintf(fp, "ocaid title author publisher year page_count stock openlibrary_edition openlibrary_work full_text_url\n");
    for (size_t i = 0; i < list->count; i++) {
        const Book *b = &list->items[i];
        write_escaped_token(fp, b->ocaid); fputc(' ', fp);
        write_escaped_token(fp, b->title); fputc(' ', fp);
        write_escaped_token(fp, b->author); fputc(' ', fp);
        write_escaped_token(fp, b->publisher); fputc(' ', fp);
        fprintf(fp, "%d %d %d ", b->year, b->page_count, b->stock);
        write_escaped_token(fp, b->openlibrary_edition); fputc(' ', fp);
        write_escaped_token(fp, b->openlibrary_work); fputc(' ', fp);
        write_escaped_token(fp, b->full_text_url); fputc('\n', fp);
    }

    fclose(fp);
    return 1;
}

int find_book_index_by_id(const BookList *list, const char *ocaid) {
    if (!list || !ocaid) return -1;
    for (size_t i = 0; i < list->count; i++) {
        if (strcmp(list->items[i].ocaid, ocaid) == 0) {
            return (int)i;
        }
    }
    return -1;
}

Book *find_book_by_id(BookList *list, const char *ocaid) {
    int idx = find_book_index_by_id(list, ocaid);
    if (idx == -1) return NULL;
    return &list->items[idx];
}

int add_book(BookList *list, Book new_book) {
    if (find_book_index_by_id(list, new_book.ocaid) != -1) {
        return 0; /* Gagal: ID buku sudah ada */
    }
    if (new_book.publisher[0] == '\0') {
        snprintf(new_book.publisher, sizeof(new_book.publisher), "%s", DEFAULT_PUBLISHER);
    }
    if (new_book.openlibrary_edition[0] == '\0') {
        snprintf(new_book.openlibrary_edition, sizeof(new_book.openlibrary_edition), "-");
    }
    if (new_book.openlibrary_work[0] == '\0') {
        snprintf(new_book.openlibrary_work, sizeof(new_book.openlibrary_work), "-");
    }
    if (new_book.full_text_url[0] == '\0') {
        snprintf(new_book.full_text_url, sizeof(new_book.full_text_url), "-");
    }

    push_book_memory(list, new_book);
    return save_books(FILE_BOOKS, list);
}

int delete_book_by_id(BookList *list, const char *ocaid) {
    int idx = find_book_index_by_id(list, ocaid);
    if (idx == -1) return 0;

    for (size_t i = (size_t)idx; i + 1 < list->count; i++) {
        list->items[i] = list->items[i + 1];
    }
    list->count--;
    return save_books(FILE_BOOKS, list);
}

/* =========================================================================
 * BAGIAN 4: PENGELOLAAN DATABASE AKUN (user.txt)
 * ========================================================================= */

int load_users(const char *filename, UserList *list) {
    FILE *fp = fopen(filename, "r");
    list->count = 0;

    if (!fp) {
        /* Buat akun bawaan jika user.txt belum ada */
        User default_admin = {"admin", "admin_loh_ya", "admin"};
        User default_user  = {"member", "penghitaman", "user"};
        push_user_memory(list, default_admin);
        push_user_memory(list, default_user);
        return save_users(filename, list);
    }

    char line[256];
    int line_number = 0;
    while (fgets(line, sizeof(line), fp)) {
        line_number++;
        User u;
        if (sscanf(line, "%63s %63s %15s", u.username, u.password, u.role) == 3) {
            if (line_number == 1 && strcmp(u.username, "username") == 0) {
                continue;
            }
            push_user_memory(list, u);
        }
    }

    fclose(fp);
    return 1;
}

int save_users(const char *filename, const UserList *list) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        fprintf(stderr, "[ERROR] Gagal menyimpan data akun ke '%s'.\n", filename);
        return 0;
    }

    fprintf(fp, "username password role\n");
    for (size_t i = 0; i < list->count; i++) {
        fprintf(fp, "%s %s %s\n",
                list->items[i].username,
                list->items[i].password,
                list->items[i].role);
    }

    fclose(fp);
    return 1;
}

User *find_user_by_username(UserList *list, const char *username) {
    if (!list || !username) return NULL;
    for (size_t i = 0; i < list->count; i++) {
        if (strcmp(list->items[i].username, username) == 0) {
            return &list->items[i];
        }
    }
    return NULL;
}

int add_user(UserList *list, User new_user) {
    if (find_user_by_username(list, new_user.username) != NULL) {
        return 0; /* Username sudah terdaftar */
    }
    push_user_memory(list, new_user);
    return save_users(FILE_USERS, list);
}

/* =========================================================================
 * BAGIAN 5: PENGELOLAAN DATABASE TRANSAKSI (peminjaman.txt)
 * ========================================================================= */

int load_transactions(const char *filename, TransactionList *list) {
    FILE *fp = fopen(filename, "r");
    list->count = 0;

    if (!fp) {
        /* Buat file peminjaman.txt kosong dengan header jika belum ada */
        return save_transactions(filename, list);
    }

    char line[512];
    while (fgets(line, sizeof(line), fp)) {
        Transaction trx;
        if (sscanf(line, "%d %63s %127s %23s %15s",
                   &trx.id_transaksi,
                   trx.username,
                   trx.ocaid,
                   trx.tanggal_pinjam,
                   trx.status) == 5) {
            push_transaction_memory(list, trx);
        }
    }

    fclose(fp);
    return 1;
}

int save_transactions(const char *filename, const TransactionList *list) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        fprintf(stderr, "[ERROR] Gagal menyimpan riwayat transaksi ke '%s'.\n", filename);
        return 0;
    }

    fprintf(fp, "id_transaksi username ocaid tanggal_pinjam status\n");
    for (size_t i = 0; i < list->count; i++) {
        const Transaction *t = &list->items[i];
        fprintf(fp, "%d %s %s %s %s\n",
                t->id_transaksi,
                t->username,
                t->ocaid,
                t->tanggal_pinjam,
                t->status);
    }

    fclose(fp);
    return 1;
}

int get_next_transaction_id(const TransactionList *list) {
    int max_id = 0;
    for (size_t i = 0; i < list->count; i++) {
        if (list->items[i].id_transaksi > max_id) {
            max_id = list->items[i].id_transaksi;
        }
    }
    return max_id + 1;
}

int add_transaction(TransactionList *list, Transaction new_trx) {
    if (new_trx.id_transaksi <= 0) {
        new_trx.id_transaksi = get_next_transaction_id(list);
    }
    push_transaction_memory(list, new_trx);
    return save_transactions(FILE_TRANSACTIONS, list);
}

Transaction *find_transaction_by_id(TransactionList *list, int id_transaksi) {
    if (!list) return NULL;
    for (size_t i = 0; i < list->count; i++) {
        if (list->items[i].id_transaksi == id_transaksi) {
            return &list->items[i];
        }
    }
    return NULL;
}

int update_transaction_status(TransactionList *list, int id_transaksi, const char *new_status) {
    Transaction *trx = find_transaction_by_id(list, id_transaksi);
    if (!trx) return 0;

    snprintf(trx->status, sizeof(trx->status), "%s", new_status);
    return save_transactions(FILE_TRANSACTIONS, list);
}

int is_book_currently_borrowed(const TransactionList *list, const char *ocaid) {
    if (!list || !ocaid) return 0;
    for (size_t i = 0; i < list->count; i++) {
        if (strcmp(list->items[i].ocaid, ocaid) == 0 &&
            strcmp(list->items[i].status, STATUS_DIPINJAM) == 0) {
            return 1;
        }
    }
    return 0;
}