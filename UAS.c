#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void lihatAlat() {
    FILE *f = fopen("item.txt", "r");
    char line[200];

    if (!f) {
        printf("File item.txt tidak ditemukan.\n");
        return;
    }

    printf("\n=== DAFTAR ALAT LAB ===\n");
    while (fgets(line, sizeof(line), f)) {
        printf("%s", line);
    }

    fclose(f);
}
