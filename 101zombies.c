#include <stdio.h>
#include <stdlib.h>  // for rand()
#include <time.h>    // for seeding random numbers
#include <string.h>  // for strcmp if needed

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

//int main() {
//    srand(time(NULL));  // seed RNG
//   printf("Zombie hits you for %d damage!\n", random_pain());
//    return 0;
//}

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

const char* weapon[] = {
    "a machete", "a chainsaw", "a shotgun", "an AK-47",
    "a minigun", "a flamethrower", "double flamethrowers", "the death ray"
};

const char* luck[] = {
    "you got no chance in hell.",
    "odds are slim.",
    "you’d better run.",
    "it’s dicey at best.",
    "it’ll be a tough fight.",
    "it’s a toss up.",
    "you’ve seen worse.",
    "the odds are with you.",
    "this shouldn’t be a problem.",
    "easy.",
    "you can’t lose!"
};

const char* emo[] = {
    "dead",
    "very fucked up",
    "fucked up",
    "like total shit",
    "like shit",
    "wounded",
    "banged up",
    "alright",
    "great",
    "fuckin’ great",
    "godlike"
};

int get_player_choice() {
    char input[2];  // buffer for user input

    while (1) {
        printf("You (f)ight or (r)un? ");
        scanf("%1s", input);  // read string input, max 1 char

        if (input[0] == 'f') {
            int roll = rand() % 100 + 1;
            return (roll <= 50) ? 1 : 2;  // fake simplified chance
        } else if (input[0] == 'r') {
            int roll = rand() % 100 + 1;
            return (roll <= 50) ? 3 : 4;
        } else {
            printf("Invalid choice. Try again.\n");
        }
    }
}

int main() {
    srand(time(NULL));  // seed random once

    int health = 100;
    int horde = 101;
    int w = 0;
    int zombies = 0;
    float fatigue = 0;

    show_help();

    while (1) {
        // Choose zombie count
        zombies = rand() % (w + 10) + 1;
        if (zombies > horde) zombies = horde;

        printf("\n%d ", horde);
        for (int i = 0; i < zombies; i++) printf(".");
        printf("\n");

        printf("%d zombies approach.\n", zombies);

        int clamped = clamp_health(health);
        int mood = clamped / 10;
        if (mood > 10) mood = 10;
        printf("You’re feeling %s.\n", emo[mood]);
        printf("Armed with %s\n", weapon[w]);

        float roll = calculate_prob(health, w, zombies);
        int y = (int)(roll / 10);
        if (y > 10) y = 10;
        printf("%s\n", luck[y]);

        int action = get_player_choice();

        // ACTION HANDLING
        if (action == 1) {
            fatigue += 1.5f;
            horde -= zombies;
            printf("You fight and win!\n");
            if (rand() % 100 > 70 && w < 7) {
                w++;
                printf("You found %s! +%d\n", weapon[w], w);
            }
        } else if (action == 2) {
            fatigue += 2.5f;
            health -= random_pain() + (int)fatigue;
            horde -= zombies;
            if (health > 0) {
                printf("You take damage, but kill them all!\n");
            }
        } else if (action == 3) {
            printf("You retreat unscathed!\n");
            health += use_medikit();
        } else if (action == 4) {
            health -= random_pain();
            if (health > 0) {
                printf("You barely get out alive!\n");
            }
        }

        // Check win/lose
        if (horde <= 0) {
            printf("You slaughtered them all. You're a HERO!\n");
            break;
        } else if (health <= 0) {
            printf("You collapse from your wounds and turn into a zombie. Game over.\n");
            break;
        }
    }

    return 0;
}





