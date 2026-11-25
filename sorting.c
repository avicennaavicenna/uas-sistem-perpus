#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "sorting.h"
#include <stdlib.h>

/* Membandingkan dua string tanpa membedakan huruf besar/kecil */
static int bandingkanTeksCaseInsensitive(const char *s1, const char *s2) {
    if (s1 == NULL && s2 == NULL) return 0;
    if (s1 == NULL) return -1;
    if (s2 == NULL) return 1;

    while (*s1 != '\0' && *s2 != '\0') {
        unsigned char c1 = (unsigned char)tolower((unsigned char)*s1);
        unsigned char c2 = (unsigned char)tolower((unsigned char)*s2);

        if (c1 != c2) {
            return (c1 < c2) ? -1 : 1;
        }
        s1++;
        s2++;
    }

    if (*s1 == '\0' && *s2 == '\0') return 0;
    return (*s1 == '\0') ? -1 : 1;
}

/* Membandingkan dua buku berdasarkan atribut */
static int bandingkanBuku(Book a, Book b, SortingField atribut) {
    switch (atribut) {
        case SORT_ID:
            return strcmp(a.ocaid, b.ocaid);

        case SORT_JUDUL:
            return bandingkanTeksCaseInsensitive(a.title, b.title);

        case SORT_TAHUN:
            if (a.year < b.year) return -1;
            if (a.year > b.year) return 1;
            return 0;

        case SORT_HALAMAN:
            if (a.page_count < b.page_count) return -1;
            if (a.page_count > b.page_count) return 1;
            return 0;

        case SORT_STOK:
            if (a.stock < b.stock) return -1;
            if (a.stock > b.stock) return 1;
            return 0;

        default:
            return 0;
    }
}

/* Menentukan apakah buku A harus berada sebelum buku B */
static int harusDiDepan(Book a, Book b, SortingField atribut, SortingOrder urutan) {
    int hasil = bandingkanBuku(a, b, atribut);

    if (urutan == ASCENDING) {
        return hasil > 0;
    }

    return hasil < 0;
}

static void bubbleSort(BookList *list, SortingField atribut, SortingOrder urutan) {
    for (size_t i = 0; i + 1 < list->count; i++) {
        for (size_t j = 0; j + 1 < list->count - i; j++) {
            if (harusDiDepan(list->items[j], list->items[j + 1], atribut, urutan)) {
                Book temp = list->items[j];
                list->items[j] = list->items[j + 1];
                list->items[j + 1] = temp;
            }
        }
    }
}

static void selectionSort(BookList *list, SortingField atribut, SortingOrder urutan) {
    for (size_t i = 0; i + 1 < list->count; i++) {
        size_t posisi = i;

        for (size_t j = i + 1; j < list->count; j++) {
            if (harusDiDepan(list->items[posisi], list->items[j], atribut, urutan)) {
                posisi = j;
            }
        }

        if (posisi != i) {
            Book temp = list->items[i];
            list->items[i] = list->items[posisi];
            list->items[posisi] = temp;
        }
    }
}

static void insertionSort(BookList *list, SortingField atribut, SortingOrder urutan) {
    for (size_t i = 1; i < list->count; i++) {
        Book kunci = list->items[i];
        size_t j = i;

        while (j > 0 && harusDiDepan(list->items[j - 1], kunci, atribut, urutan)) {
            list->items[j] = list->items[j - 1];
            j--;
        }

        list->items[j] = kunci;
    }
}
/* FUNGSI UTAMA SORTING */
void sortingBuku(BookList *daftarBuku,
                 SortingAlgorithm algoritma,
                 SortingField atribut,
                 SortingOrder urutan) {
    if (daftarBuku == NULL ||
        daftarBuku->items == NULL ||
        daftarBuku->count < 2) {
        return;
    }

    if (atribut < SORT_ID || atribut > SORT_STOK ||
        urutan < ASCENDING || urutan > DESCENDING) {
        return;
    }

    switch (algoritma) {
        case BUBBLE_SORT:
            bubbleSort(daftarBuku, atribut, urutan);
            break;

        case SELECTION_SORT:
            selectionSort(daftarBuku, atribut, urutan);
            break;

        case INSERTION_SORT:
            insertionSort(daftarBuku, atribut, urutan);
            break;

        default:
            printf("Algoritma sorting tidak valid!\n");
            break;
    }
}
/* Membuat salinan daftar buku */
int salinDaftarBuku(const BookList *src, BookList *dst) {
    if (src == NULL || dst == NULL || src->items == NULL) {
        return 0;
    }

    dst->items = NULL;
    dst->count = 0;
    dst->capacity = 0;

    if (src->count == 0) {
        return 1;
    }

    dst->items = (Book *)malloc(src->count * sizeof(Book));
    if (dst->items == NULL) {
        return 0;
    }

    memcpy(dst->items, src->items, src->count * sizeof(Book));
    dst->count = src->count;
    dst->capacity = src->count;
    return 1;
}

/* Membaca angka pilihan min-max, mengembalikan -1 jika tidak valid */
static int bacaPilihan(const char *prompt, int min, int max) {
    char input[32];
    char *end;
    long angka;

    printf("%s", prompt);

    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }

    angka = strtol(input, &end, 10);

    if (end == input) {
        return -1;
    }

    while (*end == ' ' || *end == '\t') {
        end++;
    }

    if (*end != '\n' && *end != '\0') {
        return -1;
    }

    if (angka < min || angka > max) {
        return -1;
    }

    return (int)angka;
}

/* MENU INTERAKTIF PENGURUTAN BUKU */
int menuUrutkanBuku(BookList *daftarBuku) {
    int atribut, algoritma, arah;

    if (daftarBuku == NULL || daftarBuku->count < 2) {
        return 0;
    }

    printf("\n========================================\n");
    printf("            URUTKAN KATALOG BUKU\n");
    printf("========================================\n");
    printf("Urutkan berdasarkan:\n");
    printf("1. ID Buku\n");
    printf("2. Judul Buku\n");
    printf("3. Tahun Publikasi\n");
    printf("4. Jumlah Halaman\n");
    printf("5. Stok Ketersediaan\n");
    printf("(Tekan Enter saja untuk menampilkan tanpa diurutkan)\n");

    atribut = bacaPilihan("Pilih atribut (1-5): ", 1, 5);
    if (atribut == -1) {
        printf("\nTampil tanpa pengurutan.\n");
        return 0;
    }

    printf("\nPilih algoritma pengurutan:\n");
    printf("1. Bubble Sort\n");
    printf("2. Selection Sort\n");
    printf("3. Insertion Sort\n");

    algoritma = bacaPilihan("Pilih algoritma (1-3): ", 1, 3);
    if (algoritma == -1) {
        printf("\nPilihan tidak valid.\n");
        return 0;
    }

    printf("\nPilih arah pengurutan:\n");
    if (atribut == 2) {
        printf("1. Ascending  (A-Z)\n");
        printf("2. Descending (Z-A)\n");
    } else if (atribut == 3) {
        printf("1. Ascending  (Terlama ke Terbaru)\n");
        printf("2. Descending (Terbaru ke Terlama)\n");
    } else {
        printf("1. Ascending  (Terkecil ke Terbesar)\n");
        printf("2. Descending (Terbesar ke Terkecil)\n");
    }

    arah = bacaPilihan("Pilih arah (1-2): ", 1, 2);
    if (arah == -1) {
        printf("\nPilihan tidak valid.\n");
        return 0;
    }

    sortingBuku(daftarBuku,
                (SortingAlgorithm)(algoritma - 1),
                (SortingField)(atribut - 1),
                (SortingOrder)(arah - 1));

    printf("\nBuku berhasil diurutkan.\n");
    return 1;
}
