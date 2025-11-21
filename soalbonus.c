#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_WORD_LENGTH 128

typedef struct {
    char word[MAX_WORD_LENGTH];
} WordEntry;

typedef struct {
    WordEntry *entries;
    int count;
    int capacity;
} UniqueWordList;

UniqueWordList* create_list(int initial_capacity) {
    UniqueWordList *list = (UniqueWordList*)malloc(sizeof(UniqueWordList));
    if (!list) {
        perror("Gagal alokasi memori untuk list");
        exit(EXIT_FAILURE);
    }
    list->count = 0;
    list->capacity = initial_capacity;
    list->entries = (WordEntry*)malloc(list->capacity * sizeof(WordEntry));
    if (!list->entries) {
        perror("Gagal alokasi memori untuk entri list");
        free(list);
        exit(EXIT_FAILURE);
    }
    return list;
}

void add_word(UniqueWordList *list, const char *word) {
    if (strlen(word) == 0) return;

    for (int i = 0; i < list->count; i++) {
        if (strcmp(list->entries[i].word, word) == 0) {
            return;
        }
    }

    if (list->count == list->capacity) {
        list->capacity *= 2;
        list->entries = (WordEntry*)realloc(list->entries, list->capacity * sizeof(WordEntry));
        if (!list->entries) {
            perror("Gagal re-alokasi memori");
            exit(EXIT_FAILURE);
        }
    }

    strncpy(list->entries[list->count].word, word, MAX_WORD_LENGTH - 1);
    list->entries[list->count].word[MAX_WORD_LENGTH - 1] = '\0';
    list->count++;
}

void process_and_add_word(UniqueWordList *list, char *token) {
    char processed_word[MAX_WORD_LENGTH];
    int j = 0;

    int start = 0;
    int end = strlen(token) - 1;

    while (token[start] == '\'') start++;
    while (end >= start && token[end] == '\'') end--;

    for (int i = start; i <= end && j < MAX_WORD_LENGTH - 1; i++) {
        char c = token[i];
        if (isalnum((unsigned char)c) || c == '\'') {
            processed_word[j++] = tolower((unsigned char)c);
        }
    }
    processed_word[j] = '\0';

    add_word(list, processed_word);
}

void buat_daftar_kosakata_unik(const char *file_input, const char *file_output) {
    FILE *fin = NULL;
    FILE *fout = NULL;
    char line[MAX_LINE_LENGTH];
    char title[MAX_LINE_LENGTH] = "";
    UniqueWordList *word_list = create_list(50);

    if ((fin = fopen(file_input, "r")) == NULL) {
        fprintf(stderr, "Error: Tidak dapat membuka file input '%s'\n", file_input);
        free(word_list->entries);
        free(word_list);
        return;
    }

    int is_first_line = 1;
    while (fgets(line, MAX_LINE_LENGTH, fin) != NULL) {
        line[strcspn(line, "\n")] = 0;

        if (is_first_line) {
            if (strstr(line, "[Judul:") != NULL) {
                strncpy(title, line, MAX_LINE_LENGTH - 1);
                title[MAX_LINE_LENGTH - 1] = '\0';
            }
            is_first_line = 0;
        }

        char *temp_line = strdup(line);
        if (!temp_line) {
            perror("Gagal alokasi memori temp_line");
            break;
        }

        for (char *c = temp_line; *c; c++) {
            if (!isalnum((unsigned char)*c) && *c != '\'') {
                *c = ' ';
            }
        }

        char *token = strtok(temp_line, " ");
        while (token != NULL) {
            process_and_add_word(word_list, token);
            token = strtok(NULL, " ");
        }

        free(temp_line);
    }
    
    fclose(fin);

    if ((fout = fopen(file_output, "w")) == NULL) {
        fprintf(stderr, "Error: Tidak dapat membuka file output '%s'\n", file_output);
    } else {
        if (strlen(title) > 0) {
            fprintf(fout, "%s\n", title);
        }

        for (int i = 0; i < word_list->count; i++) {
            fprintf(fout, "%s=\n", word_list->entries[i].word);
        }

        fclose(fout);
        printf("Berhasil! Daftar kosakata keren telah disimpan ke '%s' (%d kata).\n",
               file_output, word_list->count);
    }

    free(word_list->entries);
    free(word_list);
}
int main() {
    printf("--- Program Pembuat Kosakata Unik (C) ---\n");
    
    
    FILE *f_example = fopen("lirik.txt", "w");
    if (f_example) {
        fprintf(f_example, "[Judul: Perfect - Ed Sheeran]\n");
        fprintf(f_example, "I found a love for me\n");
        fprintf(f_example, "Darling, just dive right in and follow my lead\n");
        fprintf(f_example, "Well, I found a girl, beautiful and sweet\n");
        fprintf(f_example, "Oh, I never knew you were the one waiting for me\n");
        fprintf(f_example, "'Cause we were just kids when we fell in love\n");
        fprintf(f_example, "Not knowing what it was\n");
        fprintf(f_example, "I will not give you up this time\n");
        fprintf(f_example, "But darling, just kiss me slow, your heart is all I own\n");
        fprintf(f_example, "And in your eyes, you're holding mine\n");
        fprintf(f_example, "\n");
        fprintf(f_example, "Baby, I'm dancing in the dark with you between my arms\n");
        fprintf(f_example, "Barefoot on the grass, listening to our favorite song\n");
        fprintf(f_example, "When you said you looked a mess, I whispered underneath my breath\n");
        fprintf(f_example, "But you heard it, darling, you look perfect tonight\n");
        fprintf(f_example, "\n");
        fprintf(f_example, "I found a man, stronger than anyone I know\n");
        fprintf(f_example, "He shares my dreams, I hope that someday I'll share his home\n");
        fprintf(f_example, "I found a love, to carry more than just my secrets\n");
        fprintf(f_example, "To carry love, to carry children of our own\n");
        fprintf(f_example, "\n");
        fprintf(f_example, "'Cause we were just kids when we fell in love\n");
        fprintf(f_example, "Not knowing what it was\n");
        fprintf(f_example, "I will not give you up this time\n");
        fprintf(f_example, "But darling, just kiss me slow, your heart is all I own\n");
        fprintf(f_example, "And in your eyes, you're holding mine\n");
        fprintf(f_example, "\n");
        fprintf(f_example, "Baby, I'm dancing in the dark with you between my arms\n");
        fprintf(f_example, "Barefoot on the grass, listening to our favorite song\n");
        fprintf(f_example, "When you said you looked a mess, I whispered underneath my breath\n");
        fprintf(f_example, "But you heard it, darling, you look perfect tonight\n");
        fprintf(f_example, "\n");
        fprintf(f_example, "Baby, I'm dancing in the dark with you between my arms\n");
        fprintf(f_example, "Barefoot on the grass, listening to our favorite song\n");
        fprintf(f_example, "I have faith in what I see\n");
        fprintf(f_example, "Now I know I have met an angel in person\n");
        fprintf(f_example, "And she looks perfect\n");
        fprintf(f_example, "I don't deserve this\n");
        fprintf(f_example, "You look perfect tonight\n");
        fclose(f_example);
        printf("File 'lirik.txt' berhasil dibuat dengan lirik Perfect (Full Version).\n");
        printf("mantab\n");
    } else {
        fprintf(stderr, "Gagal membuat file lirik.txt.\n");
        return 1;
    }

    
    buat_daftar_kosakata_unik("lirik.txt", "kosa-kata.word");

    return 0;
}

