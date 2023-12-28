#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SET_SIZE 100

struct CardNode {
    int cardNum;
    int points;
    struct CardNode *next;
};

void initializeSet(bool set[SET_SIZE]) {
    for (int i = 0; i < SET_SIZE; i++) {
        set[i] = false;
    }
}

void addToSet(bool set[SET_SIZE], int value) {
    if (value >= 0 && value < SET_SIZE) {
        set[value] = true;
    }
}

bool isInSet(bool set[SET_SIZE], int value) {
    if (value >= 0 && value < SET_SIZE) {
        return set[value];
    } else {
        return false; // Out of range
    }
}

char *nextNumber(char *str, int index) {
    char *curr = str + index;
    while (!isdigit(*curr)) {
        curr++;
    }
    return curr;
}

int readNumber(char *curr) {
    if (!isdigit(curr[0])) {
        return -1;
    }
    int result = 0;
    int idx = 0;
    while (isdigit(curr[idx + 1])) {
        idx++;
    }
    for (int i = 0; i <= idx; i++) {
        result += (curr[i] - '0') * pow(10, idx - i);
    }
    return result;
}

void addWinningNumbersToSet(bool set[SET_SIZE], char *line) {
    char *curr = strchr(line, ':');
    curr++;
    while (*curr != '|') {
        if (isspace(*curr)) {
            curr++;
        } else if (isdigit(*curr)) {
            int num = readNumber(curr);
            while (isdigit(*curr)) {
                curr++;
            }
            addToSet(set, num);
        }
    }
}

int countWinnersOnCard(bool set[SET_SIZE], char *line) {
    int numWinners = 0;
    char *curr = strchr(line, '|');
    curr++;
    while (*curr != '\0') {
        if (isspace(*curr)) {
            curr++;
        } else if (isdigit(*curr)) {
            int num = readNumber(curr);
            while (isdigit(*curr)) {
                curr++;
            }
            if (isInSet(set, num)) {
                numWinners++;
            }
        }
    }
    return numWinners;
}

int part1() {
    FILE *file = fopen("input.txt", "r");
    if (file == NULL) {
        perror("Error opening file.");
        return 1;
    }
    char *line = NULL;
    size_t len = 0;
    struct CardNode *root = (struct CardNode *)malloc(sizeof(struct CardNode));
    struct CardNode *curr = root;
    while (getline(&line, &len, file) != -1) {
        int cardNum = readNumber(nextNumber(line, 4));
        printf("Card Num: %d\n", cardNum);
        bool set[SET_SIZE];
        initializeSet(set);
        addWinningNumbersToSet(set, line);
        int numWinners = countWinnersOnCard(set, line);
        int points = 0;
        if (numWinners > 0) {
            points = pow(2, numWinners - 1);
        }
        struct CardNode *card =
            (struct CardNode *)malloc(sizeof(struct CardNode));
        card->cardNum = cardNum;
        card->points = points;
        curr->next = card;
        curr = card;
        printf("num winners: %d\n", numWinners);
        printf("points: %d\n", points);
    }
    free(line);

    int result = 0;
    curr = root->next;
    free(root);
    while (curr != NULL) {
        result += curr->points;
        struct CardNode *temp = curr;
        curr = curr->next;
        free(temp);
    }
    printf("Total points: %d\n", result);

    return 0;
}

int part2() {
    FILE *file = fopen("input.txt", "r");
    if (file == NULL) {
        perror("Error opening file.");
        return 1;
    }
    char *line = NULL;
    size_t len = 0;
    struct CardNode *root = (struct CardNode *)malloc(sizeof(struct CardNode));
    struct CardNode *curr = root;
    while (getline(&line, &len, file) != -1) {
        int cardNum = readNumber(nextNumber(line, 4));
        printf("Card Num: %d\n", cardNum);
        bool set[SET_SIZE];
        initializeSet(set);
        addWinningNumbersToSet(set, line);
        int numWinners = countWinnersOnCard(set, line);
        struct CardNode *card =
            (struct CardNode *)malloc(sizeof(struct CardNode));
        card->cardNum = cardNum;
        card->points = numWinners;
        curr->next = card;
        curr = card;
        printf("num winners: %d\n", numWinners);
    }
    printf("last card: %d\n", curr->cardNum);
    int numCards = curr->cardNum;
    int *cardCopies = (int *)malloc(numCards * sizeof(int));
    for (int i = 0; i < numCards; i++) {
        cardCopies[i] = 1;
    }

    free(line);

    curr = root->next;
    free(root);
    while (curr != NULL) {
        int startNum = curr->cardNum;
        int numWinners = curr->points;
        int multiplier = cardCopies[startNum - 1];
        for (int i = startNum; i < startNum + numWinners; i++) {
            cardCopies[i] += multiplier;
        }
        struct CardNode *temp = curr;
        curr = curr->next;
        free(temp);
    }
    int result = 0;
    for (int i = 0; i < numCards; i++) {
        printf("arr: i: %d val: %d\n", i, cardCopies[i]);
        result += cardCopies[i];
    }
    free(cardCopies);
    printf("Total points: %d\n", result);

    return 0;
}

int main() {
    // part1();
    part2();
    return 0;
}
