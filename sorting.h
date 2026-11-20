
#ifndef SORTING_H
#define SORTING_H

#include "data_structure.h"

/* Pilihan algoritma sorting */
typedef enum {
    BUBBLE_SORT,
    SELECTION_SORT,
    INSERTION_SORT
} SortingAlgorithm;

/* Pilihan atribut buku */
typedef enum {
    SORT_ID,
    SORT_JUDUL,
    SORT_TAHUN,
    SORT_HALAMAN,
    SORT_STOK
} SortingField;

/* Urutan pengurutan */
typedef enum {
    ASCENDING,
    DESCENDING
} SortingOrder;

/* Fungsi utama sorting buku */
void sortingBuku(
    BookList *daftarBuku,
    SortingAlgorithm algoritma,
    SortingField atribut,
    SortingOrder urutan
);

#endif
