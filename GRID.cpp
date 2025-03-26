#include <stdio.h>

#include <stdlib.h>

#include <time.h>
 
// Map size

#define MAP_ROWS 10

#define MAP_COLS 10

#define MAX_SHUFFLE_STEPS 5 // Pokémon shuffle every 5 steps
 
// Player structure

typedef struct {

    int x, y;

    int hp;

    int level;

    int xp;

    int pokeballs;

    int potions;

    int steps;

} Player;
 
// Pokémon structure

typedef struct {

    char name[20];

    int hp;

    int attack;

    int x, y; // Pokémon position on the map

} Pokemon;
 
// Game variables

char gameMap[MAP_ROWS][MAP_COLS]; // Grid map

Player player = {1, 1, 30, 1, 0, 5, 2, 0}; // Player stats
 
// List of wild Pokémon

Pokemon pokemons[] = {

    {"Pikachu", 20, 5, 0, 0},

    {"Charmander", 18, 6, 0, 0},

    {"Bulbasaur", 22, 4, 0, 0},

    {"Squirtle", 25, 5, 0, 0},

    {"Eevee", 17, 6, 0, 0}

};

int totalPokemons = 5; // Number of Pokémon in the list
 
// Function prototypes

void generateMap();

void printMap();

void movePlayer(char direction);

int randomEncounter();

void battle(Pokemon *wildPokemon);

void levelUp();

void displayInventory();

void usePotion();

void catchPokemon(Pokemon *wildPokemon);

void shufflePokemonPositions();
 
int main() {

    srand(time(NULL)); // Initialize random seed

    char input;
 
    generateMap();

    shufflePokemonPositions();
 
    printf("Welcome to the Pokemon RPG in C!\n");

    printf("Move with W (Up), S (Down), A (Left), D (Right). Press Q to quit.\n\n");
 
    while (1) {

        printMap();

        printf("\nMove: ");

        scanf(" %c", &input);
 
        if (input == 'Q' || input == 'q') {

            printf("\nThanks for playing!\n");

            break;

        } else if (input == 'I' || input == 'i') {

            displayInventory();

        } else if (input == 'H' || input == 'h') {

            usePotion();

        } else {

            movePlayer(input);

            player.steps++;
 
            if (player.steps % MAX_SHUFFLE_STEPS == 0) {

                shufflePokemonPositions();

            }
 
            if (randomEncounter()) {

                int wildIndex = rand() % totalPokemons;

                battle(&pokemons[wildIndex]);

            }

        }

    }
 
    return 0;

}
 
// Function to generate the map with open areas and grass patches

void generateMap() {

	int i;

    for ( i = 0; i < MAP_ROWS; i++) {

    	int j;

        for ( j = 0; j < MAP_COLS; j++) {

            if (rand() % 4 == 0) {

                gameMap[i][j] = '|'; // Grass area (higher Pokémon encounter rate)

            } else {

                gameMap[i][j] = '_'; // Open space

            }

        }

    }

}
 
// Function to shuffle Pokémon positions on the map

void shufflePokemonPositions() {

	int i;

    for ( i = 0; i < totalPokemons; i++) {

        pokemons[i].x = rand() % MAP_COLS;

        pokemons[i].y = rand() % MAP_ROWS;

    }

    printf("\n*** The Pokemon have shuffled their locations! ***\n");

}
 
// Function to print the game map

void printMap() {

    system("clear"); // For Linux/macOS (use system("cls") on Windows)

    int i;

    for ( i = 0; i < MAP_ROWS; i++) {

    	int j;

        for ( j = 0; j < MAP_COLS; j++) {

            if (i == player.y && j == player.x) {

                printf(" P "); // Player

            } else {

                int isPokemonHere = 0;

                int k;

                for ( k = 0; k < totalPokemons; k++) {

                    if (pokemons[k].x == j && pokemons[k].y == i) {

                        printf(" * "); // Pokémon marker

                        isPokemonHere = 1;

                        break;

                    }

                }

                if (!isPokemonHere) {

                    printf(" %c ", gameMap[i][j]); // Map tiles

                }

            }

        }

        printf("\n");

    }

}
 
// Function to move the player

void movePlayer(char direction) {

    switch (direction) {

        case 'W': case 'w': if (player.y > 0) player.y--; break;

        case 'S': case 's': if (player.y < MAP_ROWS - 1) player.y++; break;

        case 'A': case 'a': if (player.x > 0) player.x--; break;

        case 'D': case 'd': if (player.x < MAP_COLS - 1) player.x++; break;

        default: printf("Invalid move!\n");

    }

}
 
// Function to check for random Pokémon encounters

int randomEncounter() {

    if (gameMap[player.y][player.x] == 'G') {

        return rand() % 3 == 0; // 33% chance in grass

    }

    return rand() % 6 == 0; // 16% chance in open area

}
 
// Function to handle Pokémon battles

void battle(Pokemon *wildPokemon) {

    int choice;

    printf("\nA wild %s appeared! HP: %d\n", wildPokemon->name, wildPokemon->hp);
 
    while (wildPokemon->hp > 0 && player.hp > 0) {

        printf("\nYour HP: %d | %s's HP: %d\n", player.hp, wildPokemon->name, wildPokemon->hp);

        printf("1. Attack\n2. Use Potion (%d left)\n3. Catch Pokemon (%d Poké Balls left)\n4. Run\nChoose: ", player.potions, player.pokeballs);

        scanf("%d", &choice);
 
        if (choice == 1) {

            printf("You attack %s!\n", wildPokemon->name);

            wildPokemon->hp -= 7;

            if (wildPokemon->hp <= 0) {

                printf("You defeated %s!\n", wildPokemon->name);

                player.xp += 10; // Gain XP

                levelUp();

                return;

            }
 
            printf("%s attacks you!\n", wildPokemon->name);

            player.hp -= wildPokemon->attack;

            if (player.hp <= 0) {

                printf("You fainted! Game Over...\n");

                exit(0);

            }

        } else if (choice == 2) {

            usePotion();

        } else if (choice == 3) {

            catchPokemon(wildPokemon);

            return;

        } else if (choice == 4) {

            printf("You ran away!\n");

            return;

        } else {

            printf("Invalid choice!\n");

        }

    }

}
 
// Function to handle player leveling up

void levelUp() {

    if (player.xp >= 20) {

        player.level++;

        player.hp += 5;

        player.xp = 0;

        printf("\n*** You Leveled Up! Now Level %d ***\n", player.level);

    }

}
 
// Function to display inventory

void displayInventory() {

    printf("\n--- Inventory ---\n");

    printf("Potions: %d\n", player.potions);

    printf("Poke Balls: %d\n", player.pokeballs);

}
 
// Function to use a potion

void usePotion() {

    if (player.potions > 0) {

        player.hp += 10;

        player.potions--;

        printf("You used a potion! HP restored.\n");

    } else {

        printf("No potions left!\n");

    }

}
 
// Function to catch a Pokémon

void catchPokemon(Pokemon *wildPokemon) {

    if (player.pokeballs > 0) {

        int success = rand() % 2; // 50% chance

        player.pokeballs--;
 
        if (success) {

            printf("You caught %s!\n", wildPokemon->name);

        } else {

            printf("The Pokemon broke free!\n");

        }

    } else {

        printf("No Poke Balls left!\n");

    }

}

 
