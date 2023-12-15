#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define ALPHABET_SIZE 26

// Trie Node
struct TrieNode {
    struct TrieNode *children[ALPHABET_SIZE];
    bool isEndOfWord;
};

// Function to initialize a new Trie Node
struct TrieNode *createNode() {
    struct TrieNode *newNode =
        (struct TrieNode *)malloc(sizeof(struct TrieNode));
    if (newNode == NULL) {
        perror("Failed to allocate memory for Trie Node");
        exit(1);
    }

    newNode->isEndOfWord = false;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        newNode->children[i] = NULL;
    }
    return newNode;
}

// Function to insert a word into the Trie
void insert(struct TrieNode *root, const char *word) {
    struct TrieNode *currentNode = root;
    for (int i = 0; word[i] != '\0'; i++) {
        int index = word[i] - 'a';
        if (currentNode->children[index] == NULL) {
            currentNode->children[index] = createNode();
        }
        currentNode = currentNode->children[index];
    }
    currentNode->isEndOfWord = true;
}

// Function to search for a word in the Trie
bool search(struct TrieNode *root, const char *word) {
    struct TrieNode *currentNode = root;
    for (int i = 0; word[i] != '\0'; i++) {
        int index = word[i] - 'a';
        if (currentNode->children[index] == NULL) {
            return false;
        }
        currentNode = currentNode->children[index];
    }
    return currentNode->isEndOfWord;
}

int main() {
    struct TrieNode *root = createNode();

    // Insert words into the Trie
    insert(root, "apple");
    insert(root, "banana");
    insert(root, "app");
    insert(root, "bat");

    // Search for words in the Trie
    printf("Search results:\n");
    printf("apple: %s\n", search(root, "apple") ? "Found" : "Not found");
    printf("app: %s\n", search(root, "app") ? "Found" : "Not found");
    printf("banana: %s\n", search(root, "banana") ? "Found" : "Not found");
    printf("bat: %s\n", search(root, "bat") ? "Found" : "Not found");
    printf("orange: %s\n", search(root, "orange") ? "Found" : "Not found");

    // Don't forget to free the memory allocated for the Trie
    // (Not implemented in this basic example)
    return 0;
}
