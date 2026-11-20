#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

#include "circulation.h"
#include "database.h"

// =====================================================
// FUNGSI BANTU: MEMBACA INPUT TEKS
// =====================================================

static void readInput(char *text, size_t size) {
    if (fgets(text, size, stdin) != NULL) {
        text[strcspn(text, "\n")] = '\0';
    } else {
        text[0] = '\0';
    }
}

// =====================================================
// FUNGSI BANTU: MEMBANDINGKAN TEKS
// Tidak membedakan huruf besar dan kecil
// =====================================================

static int containsIgnoreCase(const char *text, const char *keyword) {

    if (keyword[0] == '\0') {
        return 0;
    }

    size_t textLength = strlen(text);
    size_t keyLength = strlen(keyword);

    if (keyLength > textLength) {
        return 0;
    }

    for (size_t i = 0; i <= textLength - keyLength; i++) {

        int match = 1;

        for (size_t j = 0; j < keyLength; j++) {

            if (tolower((unsigned char)text[i + j]) !=
                tolower((unsigned char)keyword[j])) {

                match = 0;
                break;
            }
        }

        if (match) {
            return 1;
        }
    }

    return 0;
}