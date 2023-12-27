#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Game {
    int gameNumber;
    int redCount;
    int blueCount;
    int greenCount;
    int gamePower;
};

struct Draws {
    int redCount;
    int blueCount;
    int greenCount;
};

struct GameNode {
    struct Game *game;
    struct GameNode *next;
};

struct GameNode *createNode(struct Game *game) {
    struct GameNode *newNode =
        (struct GameNode *)malloc(sizeof(struct GameNode));
    if (newNode == NULL) {
        perror("Failed to allocate memory for a new node");
        exit(1);
    }
    newNode->game = game;
    newNode->next = NULL;
    return newNode;
}

void remove_trailing_commas(char *str) {
    size_t len = strlen(str);
    while (len > 0 && str[len - 1] == ',') {
        str[len - 1] = '\0'; // Replace the trailing comma with a null character
        len--;
    }
}

int readNumber(char *str, int startIndex) {
    int endIndex;
    int result = 0;
    for (int i = startIndex; isdigit(str[i]); i++) {
        endIndex = i;
    }
    for (int i = startIndex; i <= endIndex; i++) {
        result += (str[i] - '0') * pow(10, endIndex - i);
    }
    return result;
}

struct Draws *parseDraws(char *str) {
    struct Draws *draws = (struct Draws *)malloc(sizeof(struct Draws));
    int number = 0;
    for (int i = 0; str[i] != ';' && str[i] != '\0'; i++) {
        if (isdigit(str[i])) {
            number = readNumber(str, i);
            while (isdigit(str[i + 1])) {
                i++;
            }
        } else if (isalpha(str[i])) {
            if (str[i] == 'r') {
                draws->redCount = number;
            } else if (str[i] == 'b') {
                draws->blueCount = number;
            } else if (str[i] == 'g') {
                draws->greenCount = number;
            }
            while (isalpha(str[i + 1])) {
                i++;
            }
        } else if (str[i] == ',') {
            number = 0;
        }
    }
    return draws;
}

int colorcmp(char *str, char *color) {
    size_t len = strlen(str);
    size_t colorLen = strlen(color);
    if (len < colorLen) {
        return -1;
    }
    for (int i = 0; color[i] != '\0'; i++) {
        if (str[i] != color[i]) {
            return -1;
        }
    }
    return 1;
}

int main() {
    // FILE *file = fopen("sample.txt", "r");
    FILE *file = fopen("input.txt", "r");
    if (file == NULL) {
        perror("Error opening file.");
        return 1;
    }

    int redMax = 12;
    int greenMax = 13;
    int blueMax = 14;

    char *line = NULL;
    size_t len = 0;

    struct Game *game = (struct Game *)malloc(sizeof(struct Game));
    game->gameNumber = 0;
    game->redCount = 0;
    game->greenCount = 0;
    game->blueCount = 0;
    struct GameNode *rootNode =
        (struct GameNode *)malloc(sizeof(struct GameNode));
    rootNode->game = game;
    struct GameNode *currentNode = rootNode;
    int gameNumber;
    while (getline(&line, &len, file) != -1) {
        int fail = 0;
        gameNumber = readNumber(line, 5);
        struct Game *currentGame = (struct Game *)malloc(sizeof(struct Game));
        currentGame->gameNumber = gameNumber;
        currentGame->redCount = 0;
        currentGame->greenCount = 0;
        currentGame->blueCount = 0;

        char *startChar = strchr(line, ':');
        printf("Game #%d:\n", gameNumber);
        do {
            startChar++;
            struct Draws *draws = parseDraws(startChar);
            if (draws->redCount > currentGame->redCount) {
                currentGame->redCount = draws->redCount;
            }
            if (draws->greenCount > currentGame->greenCount) {
                currentGame->greenCount = draws->greenCount;
            }
            if (draws->blueCount > currentGame->blueCount) {
                currentGame->blueCount = draws->blueCount;
            }
        } while ((startChar = strchr(startChar, ';')) != NULL && !fail);
        currentGame->gamePower = currentGame->redCount *
                                 currentGame->greenCount *
                                 currentGame->blueCount;

        struct GameNode *passingNode =
            (struct GameNode *)malloc(sizeof(struct GameNode));
        passingNode->game = currentGame;
        currentNode->next = passingNode;
        currentNode = passingNode;
    }
    int result = 0;
    currentNode = rootNode;
    while (currentNode != NULL) {
        // printf("adding: %d\n", currentNode->game->gameNumber);
        result += currentNode->game->gamePower;
        printf("current total: %d\n", result);
        currentNode = currentNode->next;
    }
    printf("Final result: %d\n", result);
}
