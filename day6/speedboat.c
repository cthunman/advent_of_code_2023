#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NUM_RACES 10

struct Race {
    long racenum;
    long time;
    long distance;
};

int solveRace(struct Race *race) {
    long racetime = race->time;
    long recordDistance = race->distance;
    long numsolutions = 0;
    for (long i = 0; i < racetime; i++) {
        long distance = i * (racetime - i);
        if (distance > recordDistance) {
            numsolutions++;
        }
    }
    return numsolutions;
}

int solution2() {
    long time = 61709066;
    long distance = 643118413621041;

    struct Race *race = (struct Race *)malloc(sizeof(struct Race));
    race->time = time;
    race->distance = distance;

    long result = solveRace(race);

    printf("Result: %ld\n", result);
    return 0;
}

int solution1() {
    FILE *file = fopen("input.txt", "r");
    if (file == NULL) {
        perror("Error opening file.");
        return 1;
    }
    char *timeline = NULL;
    char *distanceline = NULL;
    size_t timelen = 0;
    size_t distancelen = 0;
    struct Race *arr =
        (struct Race *)malloc(MAX_NUM_RACES * sizeof(struct Race));
    getline(&timeline, &timelen, file);
    char *start = strchr(timeline, ':');
    start++;
    int idx = 0;

    while (*start != '\0') {
        if (isdigit(*start)) {

            int num;
            int time = sscanf(start, "%d", &num);

            arr[idx].time = num;
            idx++;
            while (isdigit(*start)) {
                start++;
            }
        } else {
            start++;
        }
    }

    getline(&distanceline, &distancelen, file);
    start = strchr(distanceline, ':');
    idx = 0;
    while (*start != '\0') {
        if (isdigit(*start)) {

            int num;
            int distance = sscanf(start, "%d", &num);

            arr[idx].distance = num;
            idx++;
            while (isdigit(*start)) {
                start++;
            }
        } else {
            start++;
        }
    }

    int mult = 1;
    for (int i = 0; i < 4; i++) {
        printf("Race %d: %d %d\n", i, arr[i].time, arr[i].distance);
        int result = solveRace(&arr[i]);
        printf("Race %d: %d\n", i, result);
        mult *= result;
    }
    printf("Result: %d\n", mult);
    return 0;
}

int main() {
    // solution1();
    solution2();
    return 0;
}
