/*
 main.c
 Program: Ekstrak daftar kosa-kata unik dari lirik lagu
 Output: kosa-kata.word

 Instruksi:
 1. Simpan lirik (yang sudah kamu paste) ke file bernama "lirik.txt" di folder yang sama.
 2. Compile:
    gcc main.c -o kosa
 3. Jalankan:
    ./kosa
 4. Hasil akan tersimpan di "kosa-kata.word"

 Ketentuan yang dipenuhi:
 - Semua kata menjadi huruf kecil
 - Simpan hanya kata unik (sekali saja)
 - Urutan sesuai kemunculan pertama di teks
 - Hapus tanda baca kecuali apostrof pada kontraksi (contoh: I'm, weren't)
 - Trim apostrof yang ada di awal/akhir token (mis. 'til -> til)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORDS 20000
#define MAX_WORD_LEN 200

// Normalize UTF-8 RIGHT SINGLE QUOTATION MARK (U+2019) to ASCII apostrophe (')
char *normalize_apostrophes(const char *s) {
    size_t len = strlen(s);
    // allocate enough space (worst case same length)
    char *out = malloc(len + 1);
    if (!out) return NULL;
    size_t i = 0, j = 0;
    while (i < len) {
        unsigned char c = (unsigned char)s[i];
        if (c == 0xE2 && i + 2 < len &&
            (unsigned char)s[i+1] == 0x80 &&
            (unsigned char)s[i+2] == 0x99) {
            out[j++] = '\''; // replace with ASCII apostrophe
            i += 3;
        } else {
            out[j++] = s[i++];
        }
    }
    out[j] = '\0';
    return out;
}

// remove leading & trailing apostrophes
void trim_apostrophes(char *w) {
    int start = 0;
    int end = (int)strlen(w) - 1;
    while (start <= end && w[start] == '\'') start++;
    while (end >= start && w[end] == '\'') end--;
    if (start == 0 && end == (int)strlen(w) - 1) return; // nothing to trim
    int k = 0;
    for (int i = start; i <= end; ++i) w[k++] = w[i];
    w[k] = '\0';
}

// check presence
int exists(char arr[][MAX_WORD_LEN], int n, const char *w) {
    for (int i = 0; i < n; ++i) {
        if (strcmp(arr[i], w) == 0) return 1;
    }
    return 0;
}

int main(void) {
    const char *input = "lirik.txt";
    const char *output = "kosa-kata.word";

    FILE *fin = fopen(input, "rb");
    if (!fin) {
        fprintf(stderr, "File %s tidak ditemukan!\n", input);
        return 1;
    }

    // read whole file
    fseek(fin, 0, SEEK_END);
    long fsize = ftell(fin);
    fseek(fin, 0, SEEK_SET);
    if (fsize < 0) fsize = 0;

    char *buf = malloc(fsize + 1);
    if (!buf) {
        fprintf(stderr, "Alokasi memori gagal.\n");
        fclose(fin);
        return 1;
    }
    size_t r = fread(buf, 1, fsize, fin);
    buf[r] = '\0';
    fclose(fin);

    char *norm = normalize_apostrophes(buf);
    free(buf);
    if (!norm) {
        fprintf(stderr, "Normalisasi apostrophe gagal.\n");
        return 1;
    }

    // Extract title (first line) exactly as-is (including newline if present)
    char title[1024] = {0};
    char *nl = strchr(norm, '\n');
    char *body = NULL;
    if (nl) {
        size_t tlen = nl - norm + 1;
        if (tlen >= sizeof(title)) tlen = sizeof(title) - 1;
        strncpy(title, norm, tlen);
        title[tlen] = '\0';
        body = nl + 1;
    } else {
        // only one line
        strncpy(title, norm, sizeof(title)-1);
        title[sizeof(title)-1] = '\0';
        body = NULL;
    }

    char words[MAX_WORDS][MAX_WORD_LEN];
    int wcount = 0;

    if (body) {
        char cur[MAX_WORD_LEN];
        int cur_len = 0;

        for (size_t i = 0; body[i] != '\0'; ++i) {
            unsigned char c = (unsigned char)body[i];
            // Accept ASCII letters, digits, apostrophe
            if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') ||
                (c >= '0' && c <= '9') || c == '\'') {
                if (c >= 'A' && c <= 'Z') c = (char)tolower(c); // to lower
                if (cur_len < MAX_WORD_LEN - 1) cur[cur_len++] = (char)c;
            } else {
                if (cur_len > 0) {
                    cur[cur_len] = '\0';
                    trim_apostrophes(cur);
                    if (strlen(cur) > 0 && !exists(words, wcount, cur)) {
                        if (wcount < MAX_WORDS) {
                            strcpy(words[wcount++], cur);
                        }
                    }
                    cur_len = 0;
                }
                // skip separators
            }
        }
        // last token
        if (cur_len > 0) {
            cur[cur_len] = '\0';
            trim_apostrophes(cur);
            if (strlen(cur) > 0 && !exists(words, wcount, cur)) {
                if (wcount < MAX_WORDS) strcpy(words[wcount++], cur);
            }
        }
    }

    free(norm);

    FILE *fout = fopen(output, "w");
    if (!fout) {
        fprintf(stderr, "Gagal membuat file output %s\n", output);
        return 1;
    }

    // write title line (if any)
    if (strlen(title) > 0) fputs(title, fout);
    else fputs("[Judul: Unknown]\n", fout);

    // write words in order
    for (int i = 0; i < wcount; ++i) {
        fprintf(fout, "%s=\n", words[i]);
    }

    fclose(fout);

    printf("Selesai. %d kata unik ditulis ke %s\n", wcount, output);
    return 0;
}
