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
