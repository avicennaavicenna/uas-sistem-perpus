#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "sorting.h"

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
static int harusDiDepan(Book a, Book b,
                        SortingField atribut,
                        SortingOrder urutan) {
    int hasil = bandingkanBuku(a, b, atribut);

    if (urutan == ASCENDING) {
        return hasil > 0;
    }

    return hasil < 0;
}

/* 1. BUBBLE SORT */
static void bubbleSort(BookList *list,
                       SortingField atribut,
                       SortingOrder urutan) {
    for (size_t i = 0; i + 1 < list->count; i++) {
        for (size_t j = 0; j + 1 < list->count - i; j++) {
            if (harusDiDepan(list->items[j],
                             list->items[j + 1],
                             atribut, urutan)) {
                Book temp = list->items[j];
                list->items[j] = list->items[j + 1];
                list->items[j + 1] = temp;
            }
        }
    }
}

/* 2. SELECTION SORT */
static void selectionSort(BookList *list,
                          SortingField atribut,
                          SortingOrder urutan) {
    for (size_t i = 0; i + 1 < list->count; i++) {
        size_t posisi = i;

        for (size_t j = i + 1; j < list->count; j++) {
            if (harusDiDepan(list->items[posisi],
                             list->items[j],
                             atribut, urutan)) {
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

/* 3. INSERTION SORT */
static void insertionSort(BookList *list,
                          SortingField atribut,
                          SortingOrder urutan) {
    for (size_t i = 1; i < list->count; i++) {
        Book kunci = list->items[i];
        size_t j = i;

        while (j > 0 &&
               harusDiDepan(list->items[j - 1],
                            kunci, atribut, urutan)) {
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
