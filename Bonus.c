#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int isLetter(char c) {
    return isalpha(c) || c == '\'';  
}

int main() {
    FILE *input = fopen("data/lirik.txt", "r");
    FILE *output = fopen("data/kosa-kata.word", "w");

    if (!input) {
        printf("lirik.txt missing!\n");
        return 0;
    }

    char judul[200];
    fgets(judul, sizeof(judul), input);

    fprintf(output, "%s", judul);

    char word[100];
    char used[5000][100];
    int usedCount = 0;

    char ch;
    int idx = 0;

    while ((ch = fgetc(input)) != EOF) {
        if (isLetter(ch) || ch == '-') {
            word[idx++] = tolower(ch);
        } else {
            if (idx > 0) {
                word[idx] = '\0';

                int exists = 0;
                for (int i = 0; i < usedCount; i++) {
                    if (strcmp(used[i], word) == 0) {
                        exists = 1;
                        break;
                    }
                }

                if (!exists) {
                    strcpy(used[usedCount++], word);
                    fprintf(output, "%s=\n", word);
                }

                idx = 0;
            }
        }
    }

    fclose(input);
    fclose(output);

    printf("Generated kosa-kata.word successfully!\n");
    return 0;
}
