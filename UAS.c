#include <stdio.h>
#include <string.h>

void lihatAlat() {
    FILE *f = fopen("items.txt", "r");
    char line[200];

    if (!f) {
        printf("File items.txt tidak ditemukan.\n");
        return;
    }

    printf("\n=== DAFTAR ALAT ===\n");
    while (fgets(line, sizeof(line), f)) {
        printf("%s", line);
    }

    fclose(f);
}

void tambahAlat() {
    FILE *f = fopen("items.txt", "a");

    char id[20], nama[50], merk[50], model[50];
    int tahun, jumlah;

    printf("ID alat   : ");
    scanf("%s", id);
    printf("Nama alat : ");
    scanf("%s", nama);
    printf("Merek     : ");
    scanf("%s", merk);
    printf("Model     : ");
    scanf("%s", model);
    printf("Tahun     : ");
    scanf("%d", &tahun);
    printf("Jumlah    : ");
    scanf("%d", &jumlah);

    fprintf(f, "%s %s %s %s %d %d\n", id, nama, merk, model, tahun, jumlah);
    fclose(f);

    printf("Alat berhasil ditambahkan!\n");
}#include <stdio.h>
#include <string.h>

void lihatAlat() {
    FILE *f = fopen("items.txt", "r");
    char line[200];

    if (!f) {
        printf("File items.txt tidak ditemukan.\n");
        return;
    }

    printf("\n=== DAFTAR ALAT ===\n");
    while (fgets(line, sizeof(line), f)) {
        printf("%s", line);
    }

    fclose(f);
}

void tambahAlat() {
    FILE *f = fopen("items.txt", "a");

    char id[20], nama[50], merk[50], model[50];
    int tahun, jumlah;

    printf("ID alat   : ");
    scanf("%s", id);
    printf("Nama alat : ");
    scanf("%s", nama);
    printf("Merek     : ");
    scanf("%s", merk);
    printf("Model     : ");
    scanf("%s", model);
    printf("Tahun     : ");
    scanf("%d", &tahun);
    printf("Jumlah    : ");
    scanf("%d", &jumlah);

    fprintf(f, "%s %s %s %s %d %d\n", id, nama, merk, model, tahun, jumlah);
    fclose(f);

    printf("Alat berhasil ditambahkan!\n");
}
