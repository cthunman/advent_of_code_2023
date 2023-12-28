#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

struct CharGrid {
    int size;
    char **grid;
};
struct Coordinate {
    int row;
    int col;
};
struct Node {
    struct Coordinate *c;
    struct Node *next;
    struct Node *prev;
};
struct PartNode {
    int partNum;
    struct PartNode *next;
};

struct Coordinate *createCoordinate(int row, int col) {
    struct Coordinate *c =
        (struct Coordinate *)malloc(sizeof(struct Coordinate));
    c->row = row;
    c->col = col;
    return c;
}

int isInBounds(struct Coordinate *c, int gridSize) {
    return !(c->row < 0 || c->row == gridSize || c->col < 0 ||
             c->col == gridSize);
}

struct Node *getAdjacentDigits(struct Coordinate *c, char **grid,
                               int gridSize) {
    struct Node *root = (struct Node *)malloc(sizeof(struct Node));
    struct Node *curr = root;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            struct Coordinate *new_c = createCoordinate(c->row + i, c->col + j);
            if (isInBounds(new_c, gridSize) &&
                !(new_c->row == c->row && new_c->col == c->col) &&
                isdigit(grid[new_c->row][new_c->col])) {

                struct Node *node = (struct Node *)malloc(sizeof(struct Node));
                node->c = new_c;
                curr->next = node;
                node->prev = curr;
                curr = node;
            }
        }
    }
    curr = root->next;
    free(root);
    return curr;
}

struct Node *getFilteredNumberNodes(struct Node *node, char **grid) {
    if (node == NULL) {
        return NULL;
    }
    struct Node *root = (struct Node *)malloc(sizeof(struct Node));
    struct Node *curr = node;
    root->next = curr;
    curr->prev = root;
    while (curr->next != NULL) {
        struct Node *next = curr->next;
        if (curr->c->row == next->c->row && curr->c->col + 1 == next->c->col) {
            struct Node *temp = curr;
            curr->prev->next = next;
            curr = next;
            curr->prev = temp->prev;
            free(temp->c);
            free(temp);
        } else {
            curr = curr->next;
        }
    }
    curr = root->next;
    curr->prev = NULL;
    free(root);
    return curr;
}

int readNumberAtCoordinate(struct Coordinate *c, char **grid, int gridSize) {
    int i = c->row;
    int j = c->col;
    if (!isdigit(grid[i][j])) {
        return -1;
    }
    int start = j;
    int end = j;
    while (start - 1 >= 0 && isdigit(grid[i][start - 1])) {
        start--;
    }
    while (end + 1 < gridSize && isdigit(grid[i][end + 1])) {
        end++;
    }
    int result = 0;
    for (int k = start; k <= end; k++) {
        result += (grid[i][k] - '0') * pow(10, end - k);
    }
    return result;
}

int main() {
    // FILE *file = fopen("test.txt", "r");
    FILE *file = fopen("input.txt", "r");
    if (file == NULL) {
        perror("Error opening file.");
        return 1;
    }

    char *line = NULL;
    size_t len = 0;
    size_t linecap = 0;
    int gridSize = 0;
    while (getline(&line, &len, file) != -1) {
        gridSize++;
    }
    rewind(file);

    char **grid = (char **)malloc(gridSize * sizeof(char *));
    for (int i = 0; i < gridSize; i++) {
        grid[i] = (char *)malloc(gridSize * sizeof(char));
    }

    for (int i = 0; getline(&line, &len, file) != -1; i++) {
        for (int j = 0; j < gridSize; j++) {
            grid[i][j] = line[j];
        }
    }
    free(line);
    fclose(file);

    struct PartNode *rootPartNode =
        (struct PartNode *)malloc(sizeof(struct PartNode));
    rootPartNode->partNum = 0;
    struct PartNode *curr = rootPartNode;
    int numParts = 0;
    for (int i = 0; i < gridSize; i++) {
        for (int j = 0; j < gridSize; j++) {
            if (grid[i][j] == '*') {
                printf("loc: %d %d\n", i, j);
                numParts++;
                struct Node *n = getFilteredNumberNodes(
                    getAdjacentDigits(createCoordinate(i, j), grid, gridSize),
                    grid);
                if (n->next != NULL && n->next->next == NULL) {
                    int num1 = readNumberAtCoordinate(n->c, grid, gridSize);
                    int num2 = readNumberAtCoordinate(n->next->c, grid, gridSize);
                    struct PartNode *pnode =
                        (struct PartNode *)malloc(sizeof(struct PartNode));
                    pnode->partNum = num1 * num2;
                    curr->next = pnode;
                    curr = pnode;
                    printf("Adding number: %d\n", pnode->partNum);
                }
            }
        }
    }
    printf("numParts: %d\n", numParts);
    int result = 0;
    curr = rootPartNode;
    while (curr != NULL) {
        result += curr->partNum;
        struct PartNode *temp = curr;
        curr = curr->next;
        free(temp);
    }
    printf("Final result: %d\n", result);

    if (0) {
        struct Node *n =
            getAdjacentDigits(createCoordinate(1, 6), grid, gridSize);
        n = getFilteredNumberNodes(n, grid);
        struct Node *curr = n;
        while (curr != NULL) {
            printf("adj %d\n", grid[curr->c->row][curr->c->col] - '0');
            curr = curr->next;
        }
        printf("gridSize: %d", gridSize);
        printf("Grid:\n");
        for (int i = 0; i < gridSize; i++) {
            for (int j = 0; j < gridSize; j++) {
                printf("%c", grid[i][j]);
            }
            printf("\n");
        }
    }
}
