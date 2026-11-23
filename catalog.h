#ifndef CATALOG_H
#define CATALOG_H

#include "data_structure.h"

void showCatalog(const BookList *books);
void addBook(BookList *books);
void editBook(BookList *books);
void deleteBook(BookList *books, const TransactionList *transactions);
int hasActiveLoan(const TransactionList *transactions, const char *ocaid);

#endif /* CATALOG_H */