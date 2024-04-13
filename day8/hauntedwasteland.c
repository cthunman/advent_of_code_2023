#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct None {
    char code[3];
    char left[3];
    char right[3];
};

int solution() {
    char *line = NULL;
    size_t len = 0;
    FILE *file = fopen("test.txt", "r");
    // FILE *file = fopen("input.txt", "r");
    if (file == NULL) {
        perror("Error opening file.");
        return 1;
    }
    char turnseq[1000];
    getline(&line, &len, file);
    strcpy(turnseq, line);
    // I want to copy the contents of line to turnseq here.

    while (getline(&line, &len, file) != -1) {
        printf("%s", line);
    }
    return 0;
}

int main() {
    solution();
    return 0;
}
