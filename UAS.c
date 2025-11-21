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




void pinjamAlat(char user[]) {
    FILE *f = fopen("loans.txt", "a");
    char id[20];

    printf("Masukkan ID alat yg dipinjam: ");
    scanf("%s", id);

    fprintf(f, "%s %s\n", user, id);
    fclose(f);

    printf("✔ Peminjaman berhasil!\n");
}

void lihatPinjaman(char user[]) {
    FILE *f = fopen("loans.txt", "r");
    char u[50], id[20];

    if (!f) {
        printf("Belum ada record peminjaman.\n");
        return;
    }

    printf("\n=== PINJAMAN ANDA (%s) ===\n", user);

    while (fscanf(f, "%s %s", u, id) != EOF) {
        if (strcmp(u, user) == 0) {
            printf("ID Alat : %s\n", id);
        }
    }

    fclose(f);
}

void menuAdmin() {
    int pilih;

    do {
        printf("\n=== MENU ADMIN ===\n");
        printf("1. Lihat Alat\n");
        printf("2. Tambah Alat\n");
        printf("3. Hapus Alat\n");
        printf("4. Keluar\n");
        printf("Pilih: ");
        scanf("%d", &pilih);

        switch(pilih) {
            case 1: lihatAlat(); break;
            case 2: tambahAlat(); break;
            case 3: hapusAlat(); break;
            case 4: printf("Keluar...\n"); break;
            default: printf("Pilihan tidak valid.\n");
        }

    } while (pilih != 4);
}

void menuUser(char username[]) {
    int pilih;

    do {
        printf("\n=== MENU USER ===\n");
        printf("1. Lihat Alat\n");
        printf("2. Pinjam Alat\n");
        printf("3. Lihat Pinjaman\n");
        printf("4. Keluar\n");
        printf("Pilih: ");
        scanf("%d", &pilih);

        switch(pilih) {
            case 1: lihatAlat(); break;
            case 2: pinjamAlat(username); break;
            case 3: lihatPinjaman(username); break;
            case 4: printf("Keluar...\n"); break;
            default: printf("Pilihan tidak valid.\n");
        }

    } while (pilih != 4);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Cara pakai:\n");
        printf("program.exe <username> <password>\n");
        return 0;
    }

    char username[50], password[50], role[20];
    strcpy(username, argv[1]);
    strcpy(password, argv[2]);

    FILE *f = fopen("users.txt", "r");
    if (!f) {
        printf("File users.txt tidak ditemukan.\n");
        return 0;
    }

    int login = 0;
    char u[50], p[50], r[20];

    while (fscanf(f, "%s %s %s", u, p, r) != EOF) {
        if (strcmp(u, username) == 0 && strcmp(p, password) == 0) {
            login = 1;
            strcpy(role, r);
            break;
        }
    }

    fclose(f);

    if (!login) {
        printf("Login gagal!\n");
        return 0;
    }

    if (strcmp(role, "admin") == 0) {
        menuAdmin();
    } else {
        menuUser(username);
    }

    return 0;
}




