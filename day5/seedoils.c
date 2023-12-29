#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG 0

struct SeedNode {
    long seedNum;
    struct SeedNode *next;
};
struct SeedNode *createSeedNode(long seedNum) {
    struct SeedNode *node = (struct SeedNode *)malloc(sizeof(struct SeedNode));
    node->seedNum = seedNum;
    return node;
}
void destructSeedNode(struct SeedNode *node) {
    struct SeedNode *next = node->next;
    free(node);
    if (next != NULL) {
        destructSeedNode(next);
    }
}

struct MappingNode {
    long startRange;
    long upperRange; // Not inclusive
    long difference;
    struct MappingNode *next;
};
struct MappingNode *createMappingNode(long destination, long source,
                                      long rangeLength) {
    struct MappingNode *node =
        (struct MappingNode *)malloc(sizeof(struct MappingNode));
    node->startRange = source;
    node->upperRange = source + rangeLength;
    node->difference = destination - source;
    return node;
}
void destructMappingNode(struct MappingNode *node) {
    struct MappingNode *next = node->next;
    free(node);
    if (next != NULL) {
        destructMappingNode(next);
    }
}

void checkForOverlaps(struct MappingNode *root) {
    struct MappingNode *curr = root;
    while (curr != NULL) {
        struct MappingNode *check = curr->next;
        while (check != NULL) {
            if (curr->startRange < check->upperRange &&
                curr->upperRange > check->startRange) {
                printf("Found overlap\n");
            }
            check = check->next;
        }
        curr = curr->next;
    }
}

char *nextNumber(char *str, int index) {
    char *curr = str + index;
    while (!isdigit(*curr)) {
        if (*curr == '\0') {
            return NULL;
        }
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
struct SeedNode *readSeedList(FILE *file) {
    struct SeedNode *root = (struct SeedNode *)malloc(sizeof(struct SeedNode));
    root->seedNum = -1;
    struct SeedNode *currNode = root;

    char *line = NULL;
    size_t len = 0;
    getline(&line, &len, file);

    char *curr = strchr(line, ':');
    curr++;
    long n = -1;
    do {
        while (!isdigit(*curr) && *curr != '\0') {
            curr++;
        }
        if (*curr == '\0') {
            break;
        }

        n = readNumber(curr);
        while (isdigit(*curr)) {
            curr++;
        }
        struct SeedNode *node = createSeedNode(n);
        currNode->next = node;
        currNode = node;
    } while (n != -1 && *curr != '\0');

    getline(&line, &len, file);
    free(line);
    return root;
}
struct MappingNode *readMappings(FILE *file, char *check) {
    struct MappingNode *root =
        (struct MappingNode *)malloc(sizeof(struct MappingNode));
    root->startRange = -1;
    root->upperRange = -1;
    root->difference = 0;
    struct MappingNode *currNode = root;

    char *line = NULL;
    size_t len = 0;
    getline(&line, &len, file);
    if (!(strcmp(line, check) == 0)) {
        printf("string does not match the check\n");
    }

    int validLine = getline(&line, &len, file);
    while (!(strcmp(line, "\n") == 0) && validLine != -1) {
        char *curr1 = line;
        while (!isdigit(*curr1) && *curr1 != '\0') {
            curr1++;
        }
        char *curr2 = strchr(curr1, ' ');
        while (!isdigit(*curr2) && *curr2 != '\0') {
            curr2++;
        }
        char *curr3 = strchr(curr2, ' ');
        while (!isdigit(*curr3) && *curr3 != '\0') {
            curr3++;
        }
        long destination = readNumber(curr1);
        long source = readNumber(curr2);
        long rangeLength = readNumber(curr3);
        struct MappingNode *node =
            createMappingNode(destination, source, rangeLength);
        currNode->next = node;
        currNode = node;

        validLine = getline(&line, &len, file);
    }

    return root;
}

int valInMappingRange(long val, struct MappingNode *map) {
    if (val >= map->startRange && val < map->upperRange) {
        return 1;
    } else {
        return 0;
    }
}
struct SeedNode *getSeedMappings(struct SeedNode *seedNode,
                                 struct MappingNode *mappingNode) {
    struct SeedNode *rootSeedNode = createSeedNode(-1);

    struct SeedNode *tempSeedNode = rootSeedNode;
    while (mappingNode != NULL) {
        if (valInMappingRange(seedNode->seedNum, mappingNode)) {
            struct SeedNode *t = rootSeedNode;
            while (t != NULL) {
                if (t->seedNum == seedNode->seedNum) {
                    continue;
                }
                t = t->next;
            }
            tempSeedNode->next =
                createSeedNode(seedNode->seedNum + mappingNode->difference);
            tempSeedNode = tempSeedNode->next;
        }
        mappingNode = mappingNode->next;
    }
    if (rootSeedNode->next == NULL) {
        destructSeedNode(rootSeedNode);
        return createSeedNode(seedNode->seedNum);
    }
    tempSeedNode = rootSeedNode->next;
    rootSeedNode->next = NULL;
    destructSeedNode(rootSeedNode);
    return tempSeedNode;
}

struct SeedNode *getFullSeedMappings(struct SeedNode *rootSeedNode,
                                     struct MappingNode *mappingNode) {
    struct SeedNode *newSeedRoot = createSeedNode(-1);
    struct SeedNode *currResultNode = newSeedRoot;

    struct SeedNode *iterOldSeed = rootSeedNode;
    while (iterOldSeed != NULL) {
        currResultNode->next = getSeedMappings(iterOldSeed, mappingNode);
        while (currResultNode->next != NULL) {
            currResultNode = currResultNode->next;
        }
        iterOldSeed = iterOldSeed->next;
    }
    destructSeedNode(rootSeedNode);
    currResultNode = newSeedRoot->next;
    newSeedRoot->next = NULL;
    destructSeedNode(newSeedRoot);
    return currResultNode;
}

int part1() {
    // FILE *file = fopen("test.txt", "r");
    FILE *file = fopen("input.txt", "r");
    if (file == NULL) {
        perror("Error opening file.");
        return 1;
    }
    char *line = NULL;
    size_t len = 0;
    struct SeedNode *seedNode = readSeedList(file);

    struct MappingNode *seedToSoil = readMappings(file, "seed-to-soil map:\n");
    struct MappingNode *soilToFert =
        readMappings(file, "soil-to-fertilizer map:\n");
    struct MappingNode *fertToWater =
        readMappings(file, "fertilizer-to-water map:\n");
    struct MappingNode *waterToLight =
        readMappings(file, "water-to-light map:\n");
    struct MappingNode *lightToTemp =
        readMappings(file, "light-to-temperature map:\n");
    struct MappingNode *tempToHum =
    readMappings(file, "temperature-to-humidity map:\n");
    struct MappingNode *humToLoc =
        readMappings(file, "humidity-to-location map:\n");

    struct SeedNode *currSeed = seedNode->next;
    currSeed = getFullSeedMappings(currSeed, seedToSoil);
    currSeed = getFullSeedMappings(currSeed, soilToFert);
    currSeed = getFullSeedMappings(currSeed, fertToWater);
    currSeed = getFullSeedMappings(currSeed, waterToLight);
    currSeed = getFullSeedMappings(currSeed, lightToTemp);
    currSeed = getFullSeedMappings(currSeed, tempToHum);
    currSeed = getFullSeedMappings(currSeed, humToLoc);

    long min = -1;
    struct SeedNode *rootSeed = currSeed;
    while (currSeed != NULL) {
        if (min == -1) {
            min = currSeed->seedNum;
        }
        if (currSeed->seedNum < min) {
            min = currSeed->seedNum;
        }
        currSeed = currSeed->next;
    }
    printf("Closest Loc: %ld\n", min);
    // Too high: 345420307

    if (DEBUG) {
        // struct MappingNode *currNode = seedToSoil->next;
        // while (currNode != NULL) {
        //     printf("humToLoc: %ld %ld %ld\n", currNode->startRange,
        //            currNode->upperRange, currNode->difference);
        //     currNode = currNode->next;
        // }
        // long test = findMapValues(79, seedToSoil);
        // long test2 = findMapValues(14, seedToSoil);
        // printf("Test %ld %ld \n", test, test2);
    }

    return 0;
}

int main() {
    part1();
    return 0;
}