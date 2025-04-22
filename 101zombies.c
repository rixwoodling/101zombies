#include <stdio.h>
#include <stdlib.h>  // for rand()
#include <time.h>    // for seeding random numbers

void show_help() {
    printf("\n101 ZOMBIES\n");
    printf("Choose to FIGHT or RUN away\n");
    printf("from the zombie horde! Chance\n");
    printf("for new weapons when fighting.\n");
    printf("However, consecutive fighting\n");
    printf("increases zombie damage. Chance\n");
    printf("for medikits when running. How-\n");
    printf("ever, constant running is for\n");
    printf("weak little girls! Good luck.\n\n");
}

int random_pain() {
    return (rand() % 15) + 1;  // gives you a number between 1 and 15
}

int main() {
    srand(time(NULL));  // seed RNG
    printf("Zombie hits you for %d damage!\n", random_pain());
    return 0;
}

int use_medikit() {
    int chance = (rand() % 100) + 1;  // 1 to 100
    if (chance > 30) {
        int sizes[] = {5, 10};
        int index = rand() % 2;  // either 0 or 1
        int size = sizes[index];

        printf("Medikit found! +%d\n", size);
        return size;
    } else {
        return 0;
    }
}

