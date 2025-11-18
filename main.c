/*
 main.c
 Program: Ekstrak daftar kosa-kata unik dari lirik lagu
 Output: kosa-kata.word

 Cara compile:
   gcc main.c -o kosa

 Cara pakai:
 - Letakkan file lirik.txt di folder yang sama dengan executable.
 - Jalankan:
   ./kosa
 - Hasil akan ditulis ke file kosa-kata.word

 Catatan:
 - Baris pertama (judul) disalin apa adanya ke file output.
 - Program normalisasikan apostrof Unicode (’) menjadi ASCII (').
 - Hanya karakter alfanumerik dan apostrof (') yang dianggap bagian kata.
 - Akan menghapus apostrof diawal/akhir kata (mis. "'til" -> til) untuk menghindari token spurious.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORDS 10000
#define MAX_WORD_LEN 200

// Ganti sequence UTF-8 U+2019 (0xE2 0x80 0x99) menjadi ASCII apostrof (')
char *normalize_apostrophes(const char *s) {
    size_t len = strlen(s);
    // Allocate buffer slightly larger to be safe
    char *out = malloc(len + 1);
    if (!out) return NULL;

    size_t i = 0, j = 0;
    while (i < len) {
        // detect 0xE2 0x80 0x99
        if ( (unsigned char)s[i] == 0xE2 && i + 2 < len &&
             (unsigned char)s[i+1] == 0x80 && (unsigned char)s[i+2] == 0x99 ) {
            out[j++] = '\''; // replace with ASCII apostrophe
            i += 3;
        } else {
            out[j++] = s[i++];
        }
    }
    out[j] = '\0';
    return out;
}

// Trim leading and trailing apostrophes from word
void trim_apostrophes(char *w) {
    int start = 0;
    int end = strlen(w) - 1;
    while (start <= end && w[start] == '\'') start++;
    while (end >= start && w[end] == '\'') end--;

    if (start == 0 && end == (int)strlen(w) - 1) {
        return; // nothing to trim
    }
    // shift substring [start..end] to beginning
    int k = 0;
    for (int i = start; i <= end; ++i) w[k++] = w[i];
    w[k] = '\0';
}

// Check if word exists in array
int exists(char words[][MAX_WORD_LEN], int count, const char *w) {
    for (int i = 0; i < count; ++i) {
        if (strcmp(words[i], w) == 0) return 1;
    }
    return 0;
}

int main(void) {
    const char *input_name = "lirik.txt";
    const char *output_name = "kosa-kata.word";

    FILE *fin = fopen(input_name, "rb");
    if (!fin) {
        fprintf(stderr, "File %s tidak ditemukan!\n", input_name);
        return 1;
    }

    // Read whole file into memory
    fseek(fin, 0, SEEK_END);
    long fsize = ftell(fin);
    fseek(fin, 0, SEEK_SET);

    char *buffer = malloc(fsize + 1);
    if (!buffer) {
        fprintf(stderr, "Memori tidak cukup.\n");
        fclose(fin);
        return 1;
    }
    size_t r = fread(buffer, 1, fsize, fin);
    buffer[r] = '\0';
    fclose(fin);

    // Normalize unicode apostrophes to ASCII apostrophe
    char *norm = normalize_apostrophes(buffer);
    free(buffer);
    if (!norm) {
        fprintf(stderr, "Gagal normalisasi.\n");
        return 1;
    }

    // Separate first line (title)
    char *rest = strchr(norm, '\n');
    char title[1024];
    size_t title_len = 0;
    if (rest) {
        title_len = rest - norm + 1; // include newline
        if (title_len >= sizeof(title)) title_len = sizeof(title) - 1;
        strncpy(title, norm, title_len);
        title[title_len] = '\0';
        rest = rest + 1; // point to after newline
    } else {
        // file has only one line
        strncpy(title, norm, sizeof(title) - 1);
        title[sizeof(title)-1] = '\0';
        rest = NULL;
    }

    // Prepare storage for unique words
    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count = 0;

    // Parse the remainder (rest)
    if (rest) {
        char cur[MAX_WORD_LEN];
        int cur_len = 0;

        for (size_t i = 0; rest[i] != '\0'; ++i) {
            unsigned char c = (unsigned char)rest[i];
            // Accept ASCII letters, digits, and apostrophe
            if ( (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') ||
                 (c >= '0' && c <= '9') || c == '\'' ) {
                // convert uppercase to lowercase (for ASCII)
                if (c >= 'A' && c <= 'Z') c = (char)tolower(c);
                if (cur_len < MAX_WORD_LEN - 1) cur[cur_len++] = (char)c;
            } else {
                // separator: terminate current word if any
                if (cur_len > 0) {
                    cur[cur_len] = '\0';
                    // remove leading/trailing apostrophes
                    trim_apostrophes(cur);
                    if (strlen(cur) > 0 && !exists(words, word_count, cur)) {
                        if (word_count < MAX_WORDS) {
                            strcpy(words[word_count++], cur);
                        }
                    }
                    cur_len = 0;
                }
                // else continue
            }
        }
        // last token
        if (cur_len > 0) {
            cur[cur_len] = '\0';
            trim_apostrophes(cur);
            if (strlen(cur) > 0 && !exists(words, word_count, cur)) {
                if (word_count < MAX_WORDS) {
                    strcpy(words[word_count++], cur);
                }
            }
        }
    }

    free(norm);

    // Write output file
    FILE *fout = fopen(output_name, "w");
    if (!fout) {
        fprintf(stderr, "Gagal membuat file output %s\n", output_name);
        return 1;
    }

    // Write title line exactly as in input
    fputs(title, fout);

    // Each word followed by '=' and newline
    for (int i = 0; i < word_count; ++i) {
        fprintf(fout, "%s=\n", words[i]);
    }

    fclose(fout);

    printf("Selesai. File '%s' berhasil dibuat dengan %d kata unik.\n", output_name, word_count);
    return 0;
}
