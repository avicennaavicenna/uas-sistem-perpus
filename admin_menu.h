#ifndef ADMIN_MENU_H
#define ADMIN_MENU_H

#include "data_structure.h"

void adminMenu(BookList *books, TransactionList *transactions);
void showTransactionReport(const BookList *books, const TransactionList *transactions);

#endif /* ADMIN_MENU_H */