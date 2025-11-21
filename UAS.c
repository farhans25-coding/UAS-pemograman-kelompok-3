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


void tambahAlat() {
    FILE *f = fopen("item.txt", "a");

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

    fprintf(f, "%s %s %s %s %d %d\n",
            id, nama, merk, model, tahun, jumlah);

    fclose(f);

    printf("✔ Alat berhasil ditambahkan!\n");
}

void hapusAlat() {
    FILE *f = fopen("item.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    char target[20];
    char id[20], nama[50], merk[50], model[50];
    int tahun, jumlah;
    int found = 0;

    if (!f) {
        printf("File item.txt tidak ditemukan.\n");
        return;
    }

    printf("Masukkan ID alat yg mau dihapus: ");
    scanf("%s", target);

    while (fscanf(f, "%s %s %s %s %d %d",
                  id, nama, merk, model, &tahun, &jumlah) != EOF) {

        if (strcmp(id, target) != 0) {
            fprintf(temp, "%s %s %s %s %d %d\n",
                    id, nama, merk, model, tahun, jumlah);
        } else {
            found = 1;
        }
    }

    fclose(f);
    fclose(temp);

    remove("item.txt");
    rename("temp.txt", "item.txt");
 if (found)
        printf("✔ Alat berhasil dihapus!\n");
    else
        printf("ID tidak ditemukan.\n");
}


