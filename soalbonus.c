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


