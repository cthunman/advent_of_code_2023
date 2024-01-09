#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NUM_HANDS 1000

struct Hand {
    char *cards;
    int bid;
    int counts[14];
};

int cardIndex(char c) {
    // char str[] = "AKQJT98765432";
    char str[] = "23456789TJQKA";
    char *result = strchr(str, c);
    if (result == NULL) {
        printf("This is all messed up.");
        return -1;
    }

    return result - str;
}

int solution1() {
    struct Hand *arr =
        (struct Hand *)malloc(MAX_NUM_HANDS * sizeof(struct Hand));

    char *line = NULL;
    size_t len = 0;
    FILE *file = fopen("input.txt", "r");
    if (file == NULL) {
        perror("Error opening file.");
        return 1;
    }
    int idx = 0;
    while (getline(&line, &len, file) != -1) {
        char *hand;
        int bid;
        int scan = sscanf(line, "%s %i", &hand, &bid);
        if (scan != 2) {
            printf("????\n");
        } else {
            printf("we are cooking\n");
        }
        idx++;
    }

    return 0;
}

int main() {
    solution1();
    return 0;
}