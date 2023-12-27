#include <stdio.h>

struct CharGrid {
    char grid[][];
};

int main() {
    FILE *file = fopen("input.txt", "r");
    if (file == NULL) {
        perror("Error opening file.");
        return 1;
    }
}