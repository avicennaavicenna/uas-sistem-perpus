#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "admin_menu.h"
#include "catalog.h"
#include "database.h"

static void pauseScreen(void)
{
    char enter[8];

    printf("\nTekan Enter untuk melanjutkan...");
    fgets(enter, sizeof(enter), stdin);
}
