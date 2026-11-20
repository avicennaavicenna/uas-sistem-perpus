#ifndef CATALOG_H
#define CATALOG_H
#include "data_structure.h"

void showCatalog(void);

void addBook(void);
void editBook(void);
void deleteBook(void);

int hasActiveLoan(const char *ocaid);

#endif