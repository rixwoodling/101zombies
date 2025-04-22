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

int clamp_health(int health) {
    if (health > 100) return 100;
    if (health < 0) return 0;
    return health;
}

float calculate_prob(int health, int w, int zombies) {
    float player = ((float)health + (float)(w * 10)) / 20.0;
    float enemy = (float)(zombies * 5) / 10.0;
    
    float roll;
    if ((player + enemy) == 0.0f) {
        roll = 0.0f;
    } else {
        roll = 100.0f * player / (player + enemy);
    }

    return roll;
}


