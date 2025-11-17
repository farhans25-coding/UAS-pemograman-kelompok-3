#include <stdio.h>
#include <string.h>

void lihatAlat() {
    FILE *f = fopen("item.txt", "r");
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

    fprintf(f, "%s %s %s %s %d %d\n", id, nama, merk, model, tahun, jumlah);
    fclose(f);

    printf("Alat berhasil ditambahkan!\n");
}

void hapusAlat() {
    FILE *f = fopen("items.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    char target[20];
    char id[20], nama[50], merk[50], model[50];
    int tahun, jumlah;

    printf("Masukkan ID alat yg mau dihapus: ");
    scanf("%s", target);

    while (fscanf(f, "%s %s %s %s %d %d",
                  id, nama, merk, model, &tahun, &jumlah) != EOF) {

        if (strcmp(id, target) != 0) {
            fprintf(temp, "%s %s %s %s %d %d\n",
                    id, nama, merk, model, tahun, jumlah);
        }
    }

    fclose(f);
    fclose(temp);

    remove("items.txt");
    rename("temp.txt", "items.txt");

    printf("Alat berhasil dihapus!\n");
}

void pinjamAlat(char user[]) {
    FILE *f = fopen("loans.txt", "a");
    char id[20];

    printf("Masukkan ID alat yg dipinjam: ");
    scanf("%s", id);

    fprintf(f, "%s %s\n", user, id);
    fclose(f);

    printf("Peminjaman berhasil!\n");
}

void lihatPinjaman(char user[]) {
    FILE *f = fopen("loans.txt", "r");
    char u[30], id[20];

    printf("\n=== PINJAMAN KAMU ===\n");
    while (fscanf(f, "%s %s", u, id) != EOF) {
        if (strcmp(u, user) == 0) {
            printf("ID Alat: %s\n", id);
        }
    }
    fclose(f);
}
