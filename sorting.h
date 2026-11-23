#ifndef SORTING_H
#define SORTING_H

#include "data_structure.h"

/* Pilihan algoritma sorting manual (tanpa qsort): BUBBLE_SORT, SELECTION_SORT, INSERTION_SORT */
typedef enum {
    BUBBLE_SORT,
    SELECTION_SORT,
    INSERTION_SORT
} SortingAlgorithm;

/* Pilihan atribut buku yang menjadi dasar pengurutan */
typedef enum {
    SORT_ID,
    SORT_JUDUL,
    SORT_TAHUN,
    SORT_HALAMAN,
    SORT_STOK
} SortingField;

/* Arah pengurutan data: ASCENDING (naik) atau DESCENDING (turun) */
typedef enum {
    ASCENDING,
    DESCENDING
} SortingOrder;

/* Mengurutkan daftar buku secara manual sesuai algoritma, atribut, dan arah urutan */
void sortingBuku(BookList *daftarBuku, SortingAlgorithm algoritma, SortingField atribut, SortingOrder urutan);

#endif
