#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Game {
    int gameNumber;
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

int main() {
    FILE *file = fopen("input.txt", "r");
    if (file == NULL) {
        perror("Error opening file.");
        return 1;
    }

    int redMax = 12;
    int blueMax = 13;
    int greenMax = 14;

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
    while (getline(&line, &len, file) != -1) {
    }
}