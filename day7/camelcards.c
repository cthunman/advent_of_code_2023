#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NUM_HANDS 1001
#define NUM_CARDS 13

#define FIVE_OF_A_KIND 6
#define FOUR_OF_A_KIND 5
#define FULL_HOUSE 4
#define THREE_OF_A_KIND 3
#define TWO_PAIR 2
#define PAIR 1
#define HIGH_CARD 0

#define JOKER_MODE 1

struct Hand {
    char *cards;
    int bid;
    int counts[NUM_CARDS];
    int initialized;
};

int handRank(struct Hand *hand) {
    int i = JOKER_MODE;
    int numJokers;
    if (JOKER_MODE == 1) {
        numJokers = hand->counts[0];
        if (numJokers == 5) {
            return FIVE_OF_A_KIND;
        }
    } else {
        numJokers = 0;
    }

    for (; i < NUM_CARDS; i++) {
        if (hand->counts[i] == 5) {
            return FIVE_OF_A_KIND;
        }
        if (hand->counts[i] == 4) {
            if (numJokers == 1) {
                return FIVE_OF_A_KIND;
            }
            return FOUR_OF_A_KIND;
        }
        if (hand->counts[i] == 3) {
            for (int j = i + 1; j < NUM_CARDS; j++) {
                if (hand->counts[j] == 2) {
                    if (numJokers == 1) {
                        return FOUR_OF_A_KIND;
                    }
                    return FULL_HOUSE;
                }
            }
            if (numJokers == 1) {
                return FOUR_OF_A_KIND;
            }
            if (numJokers == 2) {
                return FIVE_OF_A_KIND;
            }
            return THREE_OF_A_KIND;
        }
        if (hand->counts[i] == 2) {
            for (int j = i + 1; j < NUM_CARDS; j++) {
                if (hand->counts[j] == 3) {
                    return FULL_HOUSE;
                }
                if (hand->counts[j] == 2) {
                    if (numJokers == 1) {
                        return FULL_HOUSE;
                    }
                    return TWO_PAIR;
                }
            }
            if (numJokers == 1) {
                return THREE_OF_A_KIND;
            }
            if (numJokers == 2) {
                return FOUR_OF_A_KIND;
            }
            if (numJokers == 3) {
                return FIVE_OF_A_KIND;
            }
            return PAIR;
        }
    }
    if (numJokers == 1) {
        return PAIR;
    }
    if (numJokers == 2) {
        return THREE_OF_A_KIND;
    }
    if (numJokers == 3) {
        return FOUR_OF_A_KIND;
    }
    if (numJokers == 4) {
        return FIVE_OF_A_KIND;
    }
    return HIGH_CARD;
}

int jokerCardIndex(char c) {
    char str[] = "J23456789TQKA";
    char *result = strchr(str, c);
    if (result == NULL) {
        return -1;
    }
    return result - str;
}

int cardIndex(char c) {
    char str[] = "23456789TJQKA";
    char *result = strchr(str, c);
    if (result == NULL) {
        return -1;
    }
    return result - str;
}

int (*getCardIndexFunction())(char) {
    if (JOKER_MODE == 1) {
        return &jokerCardIndex;
    } else {
        return &cardIndex;
    }
}

int compareHands(struct Hand *hand1, struct Hand *hand2) {
    int hand1Rank = handRank(hand1);
    int hand2Rank = handRank(hand2);
    if (hand1Rank > hand2Rank) {
        return 1;
    } else if (hand1Rank < hand2Rank) {
        return -1;
    } else {
        int h1, h2;
        int (*cardIndexFunction)(char) = getCardIndexFunction();
        for (int i = 0; i < NUM_CARDS; i++) {
            h1 = cardIndexFunction(hand1->cards[i]);
            h2 = cardIndexFunction(hand2->cards[i]);
            if (h1 > h2) {
                return 1;
            } else if (h2 > h1) {
                return -1;
            }
        }
        return 0;
    }
}

void sortArray(struct Hand *arr, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = i + 1; j < size; j++) {
            if (compareHands(&arr[i], &arr[j]) == 1) {
                struct Hand temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }
}

int solution() {
    struct Hand arr[MAX_NUM_HANDS];
    char *line = NULL;
    size_t len = 0;
    // FILE *file = fopen("test.txt", "r");
    FILE *file = fopen("input.txt", "r");
    if (file == NULL) {
        perror("Error opening file.");
        return 1;
    }
    int idx = 0;
    char hand[6];
    int (*cardIndexFunction)(char) = getCardIndexFunction();
    while (getline(&line, &len, file) != -1) {
        int bid;
        int scan = sscanf(line, "%s %i", hand, &bid);
        if (scan != 2) {
            return -1;
        } else {
            arr[idx].cards = strdup(hand);
            arr[idx].bid = bid;
            arr[idx].initialized = 1;
            for (int i = 0; i < NUM_CARDS; i++) {
                arr[idx].counts[i] = 0;
            }
            for (int i = 0; i < 5; i++) {
                arr[idx].counts[cardIndexFunction(hand[i])]++;
            }
        }
        idx++;
    }

    sortArray(arr, idx);

    int i = 0;
    int runningSum = 0;
    while (arr[i].initialized) {
        printf("%i\t%s\t%i\t", i + 1, arr[i].cards, arr[i].bid);
        int handValue = (i + 1) * (arr[i].bid);
        runningSum += handValue;
        printf("%i", handValue);
        printf("\n");
        i++;
    }
    printf("Total: %i\n", runningSum);
    return 0;
}

int main() {
    solution();
    return 0;
}