#include <stdio.h>
#include <string.h>

int main() {
    char input[1005];
    char *words[305];
    int wordCount = 0;

    fgets(input, sizeof(input), stdin);

    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }

    char *token = strtok(input, " ");
    while (token != NULL) {
        words[wordCount++] = token;
        token = strtok(NULL, " ");
    }
    
    int first = 1;
    for (int i = 0; i < wordCount; i++) {
        int sudahAda = 0;
        for (int j = 0; j < i; j++) {
            if (strcmp(words[i], words[j]) == 0) {
                sudahAda = 1;
                break;
            }
        }
        if (!sudahAda) {
            if (!first) printf(" ");
            printf("%s", words[i]);
            first = 0;
        }
    }
    printf("\n");

    return 0;
}
