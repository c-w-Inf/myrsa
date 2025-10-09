// Copyright 2025 c-w-Inf. All Rights Reserved.

#include "base64.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void decode (size_t bufsize) {
    char* buf = malloc (bufsize + 1);
    uint8_t* bufres = malloc (base64_decode_size (bufsize));
    while (fgets (buf, bufsize + 1, stdin) != NULL) {
        fwrite (bufres, 1, base64_decode (buf, bufres), stdout);
    }
}
void encode (size_t bufsize) {
    uint8_t* buf = malloc (bufsize);
    char* bufres = malloc (base64_encode_size (bufsize));
    size_t bread;
    while ((bread = fread (buf, 1, bufsize, stdin))) {
        base64_encode (buf, bread, bufres);
        printf ("%s", bufres);
    }
}

int main (int argc, char** argv) {
    if (argc == 2) {
        if (strcmp (argv[1], "-d") == 0) {
            decode (8193);
        } else if (strcmp (argv[1], "-e") == 0) {
            encode (8193);
        } else {
            printf ("unknown option: %s\n", *argv);
        }
    } else if (argc == 3) {
        size_t bufsize;
        char bufc;
        if (sscanf (argv[2], "%zd%c", &bufsize, &bufc) == 1) {
            if (strcmp (argv[1], "-d") == 0) {
                decode (bufsize / 4 * 4);
            } else if (strcmp (argv[2], "-e") == 0) {
                encode (bufsize / 3 * 3);
            } else {
                printf ("unknown option: %s\n", *argv);
            }
        } else {
            printf ("not a number: %s\n", argv[2]);
        }

    } else {
        printf ("Usage: base64 -d | -e\n");
    }

    return 0;
}
