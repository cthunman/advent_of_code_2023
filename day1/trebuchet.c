#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#define ALPHABET_SIZE 26

struct Node {
    int data;
    struct Node *next;
};

struct Node *createNode(int data) {
    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
    if (newNode == NULL) {
        perror("Failed to allocate memory for a new node");
        exit(1);
    }
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

struct TrieNode {
    int value;
    struct TrieNode *children[ALPHABET_SIZE];
};

struct TrieNode *createTrieNode() {
    struct TrieNode *newTrie =
        (struct TrieNode *)malloc(sizeof(struct TrieNode));
    if (newTrie == NULL) {
        perror("Failed to allocate memory for a new node");
        exit(1);
    }
    newTrie->value = -1;

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        newTrie->children[i] = NULL;
    }
    return newTrie;
}

void freeTrieNode(struct TrieNode *root) {
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (root->children[i] != NULL) {
            freeTrieNode(root->children[i]);
        }
    }
    free(root);
}

void trieInsert(struct TrieNode *root, const char *word, const int value) {
    struct TrieNode *currentNode = root;
    for (int i = 0; word[i] != '\0'; i++) {
        int idx = word[i] - 'a';
        if (currentNode->children[idx] == NULL) {
            currentNode->children[idx] = createTrieNode();
        }
        currentNode = currentNode->children[idx];
    }
    currentNode->value = value;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        perror("Error opening file");
        return 2;
    }

    char *line = NULL;
    size_t len = 0;
    struct Node *head = createNode(0);
    struct Node *current = head;

    struct TrieNode *rootTrie = createTrieNode();
    trieInsert(rootTrie, "one", 1);
    trieInsert(rootTrie, "two", 2);
    trieInsert(rootTrie, "three", 3);
    trieInsert(rootTrie, "four", 4);
    trieInsert(rootTrie, "five", 5);
    trieInsert(rootTrie, "six", 6);
    trieInsert(rootTrie, "seven", 7);
    trieInsert(rootTrie, "eight", 8);
    trieInsert(rootTrie, "nine", 9);

    while (getline(&line, &len, file) != -1) {
        char firstDigit = '\0';
        char lastDigit = '\0';

        for (size_t i = 0; line[i] != '\0'; i++) {
            if (isdigit(line[i])) {
                if (firstDigit == '\0') {
                    firstDigit = line[i];
                }
                lastDigit = line[i];
            } else {
                struct TrieNode *currTrie = rootTrie;
                size_t j = i;
                int idx = line[j] - 'a';
                while (currTrie->children[idx] != NULL) {
                    currTrie = currTrie->children[idx];
                    j++;
                    idx = line[j] - 'a';
                    if (idx < 0 || idx > ALPHABET_SIZE) {
                        break;
                    }
                }
                if (currTrie->value != -1) {
                    if (firstDigit == '\0') {
                        firstDigit = currTrie->value + '0';
                    }
                    lastDigit = currTrie->value + '0';
                    currTrie = rootTrie;
                }
            }
        }

        if (firstDigit != '\0' && lastDigit != '\0') {
            int firstNum = firstDigit - '0';
            int lastNum = lastDigit - '0';
            int concatNum = firstNum * 10 + lastNum;

            struct Node *next = createNode(concatNum);
            current->next = next;
            current = current->next;
        }
    }
    free(line);
    freeTrieNode(rootTrie);

    int sum = 0;
    current = head;
    while (current != NULL) {
        sum += current->data;
        struct Node *temp = current;
        current = current->next;
        free(temp);
    }

    printf("\nSum: %d\n", sum);
    fclose(file);
    return 0;
}
